#include "MapGenerator.h"
#include "SimplexNoise.h"

const int CANVAS_SIZE = 128;


MapGenerator::MapGenerator(int size)
{
	m_elevation.resize(size * size);

    const int width = 64;
    const int height = 64;
    m_colormap.resize(width * height * 4);
    for (int y = 0, p = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int e = 2 * x / width - 1,
                m = y / height;

            char r, g, b;

            if (x == width / 2 - 1) {
                r = 48;
                g = 120;
                b = 160;
            }
            else
                if (x == width / 2 - 2) {
                    r = 48;
                    g = 100;
                    b = 150;
                }
                else if (x == width / 2 - 3) {
                    r = 48;
                    g = 80;
                    b = 140;
                }
                else
                    if (e < 0.0) {
                        r = 48 + 48 * e;
                        g = 64 + 64 * e;
                        b = 127 + 127 * e;
                    }
                    else { // adapted from terrain-from-noise article
                        m = m * (1 - e); // higher elevation holds less moisture; TODO: should be based on slope, not elevation

                        r = 210 - 100 * m;
                        g = 185 - 45 * m;
                        b = 139 - 45 * m;
                        r = 255 * e + r * (1 - e),
                            g = 255 * e + g * (1 - e),
                            b = 255 * e + b * (1 - e);
                    }

            m_colormap[p++] = r;
            m_colormap[p++] = g;
            m_colormap[p++] = b;
            m_colormap[p++] = 255;
        }
    }
}

void MapGenerator::setSeedAndIsland(int seed, int island)
{
	if (m_seed == seed && m_island == island) return;
	m_seed = seed;
	m_island = island;
	generate();
}

void MapGenerator::generate()
{
    Noise2D noise2D(m_seed);
    double persistence = 0.5;
    QVector<double> amplitudes = { 1,0.5,0.25,0.125,0.0625 };

    auto fbm_noise = [&](int nx, int ny) {
        int sum = 0, sumOfAmplitudes = 0;
        for (int octave = 0; octave < amplitudes.size(); octave++) {
            int frequency = 1 << octave;
            sum += amplitudes[octave] * noise2D.get(nx * frequency, ny * frequency);
            sumOfAmplitudes += amplitudes[octave];
        }
        return sum / sumOfAmplitudes;
    };

    for (int y = 0; y < CANVAS_SIZE; y++) {
        for (int x = 0; x < CANVAS_SIZE; x++) {
            int p = y * CANVAS_SIZE + x;
            int nx = 2 * x / CANVAS_SIZE - 1,
                ny = 2 * y / CANVAS_SIZE - 1;
            int distance = std::max(abs(nx),abs(ny));
            double e = 0.5 * (fbm_noise(nx, ny) + m_island * (0.75 - 2 * distance * distance));
            if (e < -1.0) { e = -1.0; }
            if (e > +1.0) { e = +1.0; }
            m_elevation[p] = e;
            if (e > 0.0) {
                double m = (0.5 * noise2D.get(nx + 30, ny + 50)
                    + 0.5 * noise2D.get(2 * nx + 33, 2 * ny + 55));
                // TODO: make some of these into parameters
                double mountain = std::min(1.0, e * 5.0) * (1 - fabs(m) / 0.5);
                if (mountain > 0.0) {
                    m_elevation[p] = std::max(e, std::min(e * 3, mountain));
                }
            }
        }
    }
    m_userHasPainted = false;
}
