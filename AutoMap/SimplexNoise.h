#pragma once
#include <qvector.h>

/**
 * Creates a 2D noise function
 * @param random the random function that will be used to build the permutation table
 * @returns {NoiseFunction2D}
 */

class Noise2D {
public:
    Noise2D(int seed);
    ~Noise2D(){}
    /**
 * Samples the noise field in two dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y) const;
private:
    QVector<unsigned char> m_perm;
    QVector<int> m_permGrad2x;
    QVector<int> m_permGrad2y;
};


 /**
  * Creates a 3D noise function
  * @param random the random function that will be used to build the permutation table
  * @returns {NoiseFunction3D}
  */
class Noise3D {
public:
    Noise3D(int seed);
    ~Noise3D() {};
    /**
 * Samples the noise field in three dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @param z
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y, int z);
private:
    QVector<unsigned char> m_perm;
    QVector<int> m_permGrad3x;
    QVector<int> m_permGrad3y;
    QVector<int> m_permGrad3z;
};

/**
 * Creates a 4D noise function
 * @param random the random function that will be used to build the permutation table
 * @returns {NoiseFunction4D}
 */
class Noise4D
{
public:
    Noise4D(int seed);
    ~Noise4D() {};
    /**
 * Samples the noise field in four dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @param z
 * @param w
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y, int z, int w);

private:
    QVector<unsigned char> m_perm;
    QVector<int> m_permGrad4x;
    QVector<int> m_permGrad4y;
    QVector<int> m_permGrad4z;
    QVector<int> m_permGrad4w;
};
