#include "Delaunator.h"
#include "Orient2D.h"
#include "Utils.h"
#include <numeric>
#include <cmath>

const double EPSILON = pow(2, -52);
QVector<unsigned int> EDGE_STACK(512, 0);

bool inCircle(double ax, double ay, double bx, double by, double cx, double cy, double px, double py) {
    double dx = ax - px;
    double dy = ay - py;
    double ex = bx - px;
    double ey = by - py;
    double fx = cx - px;
    double fy = cy - py;

    double ap = dx * dx + dy * dy;
    double bp = ex * ex + ey * ey;
    double cp = fx * fx + fy * fy;

    return dx * (ey * cp - bp * fy) -
        dy * (ex * cp - bp * fx) +
        ap * (ex * fy - ey * fx) < 0;
}


QPointF circumcenter(double ax, double ay, double bx, double by, double cx, double cy) {
    double dx = bx - ax;
    double dy = by - ay;
    double ex = cx - ax;
    double ey = cy - ay;

    double bl = dx * dx + dy * dy;
    double cl = ex * ex + ey * ey;
    double d = 0.5 / (dx * ey - dy * ex);

    double x = ax + (ey * bl - dy * cl) * d;
    double y = ay + (dx * cl - ex * bl) * d;

    return { x, y };
}


void swap(QVector<unsigned int> &arr,int i,int j) {
    const unsigned int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

double distance(double ax, double ay, double bx, double by) {
    double dx = ax - bx;
    double dy = ay - by;
    return dx * dx + dy * dy;
}

double  circumradius(double ax, double ay, double bx, double by, double cx, double cy) {
    const double dx = bx - ax;
    const double dy = by - ay;
    const double ex = cx - ax;
    const double ey = cy - ay;

    const double bl = dx * dx + dy * dy;
    const double cl = ex * ex + ey * ey;
    const double d = 0.5 / (dx * ey - dy * ex);

    const double x = (ey * bl - dy * cl) * d;
    const double y = (dx * cl - ex * bl) * d;

    return x * x + y * y;
}

void quicksort(QVector<unsigned int> &ids, QVector<double> &dists, int left, int right) {
    if (right - left <= 20) {
        for (size_t i = left + 1; i <= right; i++) {
            const double temp = ids[i];
            const double tempDist = dists[temp];
            int j = i - 1;
            while (j >= left && dists[ids[j]] > tempDist) ids[j + 1] = ids[j--];
            ids[j + 1] = temp;
        }
    }
    else {
        const double median = (left + right) >> 1;
        double i = left + 1;
        double j = right;
        swap(ids, median, i);
        if (dists[ids[left]] > dists[ids[right]]) swap(ids, left, right);
        if (dists[ids[i]] > dists[ids[right]]) swap(ids, i, right);
        if (dists[ids[left]] > dists[ids[i]]) swap(ids, left, i);

        const unsigned int temp = ids[i];
        const double tempDist = dists[temp];
        while (true) {
            do i++; while (dists[ids[i]] < tempDist);
            do j--; while (dists[ids[j]] > tempDist);
            if (j < i) break;
            swap(ids, i, j);
        }
        ids[left + 1] = ids[j];
        ids[j] = temp;

        if (right - i + 1 >= j - left) {
            quicksort(ids, dists, i, right);
            quicksort(ids, dists, left, j - 1);
        }
        else {
            quicksort(ids, dists, left, j - 1);
            quicksort(ids, dists, i, right);
        }
    }
}


Delaunator::Delaunator(const QVector<QPointF>& data)
{
    size_t count = data.size();
    m_coords.resize(count * 2);

    for (size_t index = 0; index < count; index++) {
        m_coords[2 * index] = data[index].x();
        m_coords[2 * index + 1] = data[index].y();
    }
    const size_t n = m_coords.size() >> 2;
    const size_t maxTiangles = std::max(2 * n - 5, (size_t)0);
    m_triangles.resize(maxTiangles * 3);
    m_halfEdges.resize(maxTiangles * 3);

    m_hashSize = ceil(sqrtf(n));
    m_hullPrev.resize(n);
    m_hullNext.resize(n);
    m_hullTri.resize(n);
    m_hullHash.fill(-1, m_hashSize);

    m_ids.resize(n);
    m_dists.resize(n);

    update();
}

Delaunator::Delaunator(const Delaunator& other)
{
    m_triangles = other.m_triangles;
    m_halfEdges = other.m_halfEdges;
}

void Delaunator::update()
{
    const size_t n = m_coords.size() >> 1;
    double maxY = std::numeric_limits<double>::lowest();
    double maxX = std::numeric_limits<double>::lowest();
    double minY = std::numeric_limits<double>::infinity();
    double minX = std::numeric_limits<double>::infinity();

    for (size_t index = 0; index < n; index++) {
        const double x = m_coords[2 * index];
        const double y = m_coords[2 * index + 1];
        if (x < minX) minX = x;
        if (y < minY) minY = y;
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
        m_ids[index] = index;
    }

    double cx = minX + (maxX - minX) * 0.5;
    double cy = minY + (maxY - minY) * 0.5;

    double minDist = std::numeric_limits<double>::infinity();

    size_t i0 = 0, i1 = 0, i2 = 0;
    // pick a seed point close to the center
    for (int index = 0; index < n; index++) {
        double value = distance(cx, cy, m_coords[2 * index], m_coords[2 * index + 1]);
        if (value < minDist) {
            i0 = index;
            minDist = value;
        }
    }

    const double i0x = m_coords[2 * i0];
    const double i0y = m_coords[2 * i0 + 1];

    minDist = std::numeric_limits<double>::infinity();
    // find the point closest to the seed
    for (size_t index = 0; index < n; index++) {
        if (index ==  i0) continue;
        const double value = distance(i0x, i0y, m_coords[2 * index], m_coords[2 * index + 1]);
        if (value < minDist && value > 0) {
            i1 = index;
            minDist = value;
        }
    }
    double i1x = m_coords[2 * i1];
    double i1y = m_coords[2 * i1 + 1];

    double minRadius = std::numeric_limits<double>::infinity();

    // find the third point which forms the smallest circumcircle with the first two
    for (size_t i = 0; i < n; i++) {
        if (i == i0 || i == i1) continue;
        const double  r = circumradius(i0x, i0y, i1x, i1y, m_coords[2 * i], m_coords[2 * i + 1]);
        if (r < minRadius) {
            i2 = i;
            minRadius = r;
        }
    }
    double i2x = m_coords[2 * i2];
    double i2y = m_coords[2 * i2 + 1];

    if (minRadius == std::numeric_limits<double>::infinity()) {
        // order collinear points by dx (or dy if all x are identical)
        // and return the list as a hull
        for (size_t i = 0; i < n; i++) {
            m_dists[i] = (m_coords[2 * i] - m_coords[0]) || (m_coords[2 * i + 1] - m_coords[1]);
        }
        quicksort(m_ids, m_dists, 0, n - 1);
        QVector<unsigned int> hull(n, 0);        
        size_t j = 0;
        for (size_t i = 0, d0 = std::numeric_limits<double>::lowest(); i < n; i++) {
            const unsigned int id = m_ids[i];
            if (m_dists[id] > d0) {
                hull[j++] = id;
                d0 = m_dists[id];
            }
        }
        for (size_t index = 0; index < j; index++) m_hull.push_back(hull[index]);
        return;
    }

    // swap the order of the seed points for counter-clockwise orientation
    if (orient2d(i0x, i0y, i1x, i1y, i2x, i2y) < 0) {
        double i = i1;
        double x = i1x;
        double y = i1y;
        i1 = i2;
        i1x = i2x;
        i1y = i2y;
        i2 = i;
        i2x = x;
        i2y = y;
    }

    QPointF center = circumcenter(i0x, i0y, i1x, i1y, i2x, i2y);
    m_cx = center.x();
    m_cy = center.y();

    for (int i = 0; i < n; i++) {
        m_dists[i] = distance(m_coords[2 * i], m_coords[2 * i + 1], center.x(), center.y());
    }

    // sort the points by distance from the seed triangle circumcenter
    quicksort(m_ids, m_dists, 0, n - 1);

    // set up the seed triangle as the starting hull
    double m_hullStart = i0;
    int hullSize = 3;

    m_hullNext[i0] = m_hullPrev[i2] = i1;
    m_hullNext[i1] = m_hullPrev[i0] = i2;
    m_hullNext[i2] = m_hullPrev[i1] = i0;

    m_hullTri[i0] = 0;
    m_hullTri[i1] = 1;
    m_hullTri[i2] = 2;

    m_hullHash.fill(-1);
    m_hullHash[hashKey(i0x, i0y)] = i0;
    m_hullHash[hashKey(i1x, i1y)] = i1;
    m_hullHash[hashKey(i2x, i2y)] = i2;

    m_trianglesLen = 0;
    addTriangle(i0, i1, i2, -1, -1, -1);

    for (int k = 0, xp, yp; k < m_ids.size(); k++) {
        int i = m_ids[k];
        double x = m_coords[2 * i];
        double y = m_coords[2 * i + 1];

        // skip near-duplicate points
        if (k > 0 && abs(x - xp) <= EPSILON && abs(y - yp) <= EPSILON) continue;
        xp = x;
        yp = y;

        // skip seed triangle points
        if (i == i0 || i == i1 || i == i2) continue;

        // find a visible edge on the convex hull using edge hash
        int start = 0;
        for (int j = 0, key = hashKey(x, y); j < m_hashSize; j++) {
            start = m_hullHash[(key + j) % m_hashSize];
            if (start != -1 && start != m_hullNext[start]) break;
        }

        start = m_hullPrev[start];
        int e = start, q;
        while (q = m_hullNext[e], orient2d(x, y, m_coords[2 * e], m_coords[2 * e + 1], m_coords[2 * q], m_coords[2 * q + 1]) >= 0) {
            e = q;
            if (e == start) {
                e = -1;
                break;
            }
        }
        if (e == -1) continue; // likely a near-duplicate point; skip it

        // add the first triangle from the point
        int t = addTriangle(e, i, m_hullNext[e], -1, -1, m_hullTri[e]);

        // recursively flip triangles from the point until they satisfy the Delaunay condition
        m_hullTri[i] = legalize(t + 2);
        m_hullTri[e] = t; // keep track of boundary triangles on the hull
        hullSize++;

        // walk forward through the hull, adding more triangles and flipping recursively
        int n = m_hullNext[e];
        while (q = m_hullNext[n], orient2d(x, y, m_coords[2 * n], m_coords[2 * n + 1], m_coords[2 * q], m_coords[2 * q + 1]) < 0) {
            t = addTriangle(n, i, q, m_hullTri[i], -1, m_hullTri[n]);
            m_hullTri[i] = legalize(t + 2);
            m_hullNext[n] = n; // mark as removed
            hullSize--;
            n = q;
        }

        // walk backward from the other side, adding more triangles and flipping
        if (e == start) {
            while (q = m_hullPrev[e], orient2d(x, y, m_coords[2 * q], m_coords[2 * q + 1], m_coords[2 * e], m_coords[2 * e + 1]) < 0) {
                t = addTriangle(q, i, e, -1, m_hullTri[e], m_hullTri[q]);
                legalize(t + 2);
                m_hullTri[q] = t;
                m_hullNext[e] = e; // mark as removed
                hullSize--;
                e = q;
            }
        }

        // update the hull indices
        m_hullStart = m_hullPrev[i] = e;
        m_hullNext[e] = m_hullPrev[n] = i;
        m_hullNext[i] = n;

        // save the two new edges in the hash table
        m_hullHash[hashKey(x, y)] = i;
        m_hullHash[hashKey(m_coords[2 * e], m_coords[2 * e + 1])] = e;
    }
    m_hull.resize(hullSize);
    for (int i = 0, e = m_hullStart; i < hullSize; i++) {
        m_hull[i] = e;
        e = m_hullNext[e];
    }

    // trim typed triangle mesh arrays
    m_triangles.resize(m_trianglesLen);
    m_halfEdges.resize(m_trianglesLen);
    //m_triangles = this._triangles.subarray(0, m_trianglesLen);
    //m_halfedges = this._halfedges.subarray(0, m_trianglesLen);

}

// monotonically increases with real angle, but doesn't need expensive trigonometry
double pseudoAngle(double dx,double dy) {
    double p = dx / fabs(dx) + fabs(dy);
    return (dy > 0 ? 3 - p : 1 + p) / 4; // [0..1]
}

int Delaunator::hashKey(double x, double y)
{
    double value = pseudoAngle(x - m_cx, y - m_cy) * m_hashSize;
    return (int)floor(value) % m_hashSize;
}

int Delaunator::addTriangle(int i0, int i1, int i2, int a, int b, int c)
{
    int t = m_trianglesLen;

    m_triangles[t] = i0;
    m_triangles[t + 1] = i1;
    m_triangles[t + 2] = i2;

    link(t, a);
    link(t + 1, b);
    link(t + 2, c);

    m_trianglesLen += 3;

    return t;
}

void Delaunator::link(int a, int b)
{
    m_halfEdges[a] = b;
    if(b != -1) m_halfEdges[b] = a;
}

int Delaunator::legalize(int a)
{
    int i = 0;
    int ar = 0;

    // recursion eliminated with a fixed-size stack
    while (true) {
        int b = m_halfEdges[a];

        /* if the pair of triangles doesn't satisfy the Delaunay condition
         * (p1 is inside the circumcircle of [p0, pl, pr]), flip them,
         * then do the same check/flip recursively for the new pair of triangles
         *
         *           pl                    pl
         *          /||\                  /  \
         *       al/ || \bl            al/    \a
         *        /  ||  \              /      \
         *       /  a||b  \    flip    /___ar___\
         *     p0\   ||   /p1   =>   p0\---bl---/p1
         *        \  ||  /              \      /
         *       ar\ || /br             b\    /br
         *          \||/                  \  /
         *           pr                    pr
         */
        int a0 = a - a % 3;
        ar = a0 + (a + 2) % 3;

        if (b ==  -1) { // convex hull edge
            if (i ==  0) break;
            a = EDGE_STACK[--i];
            continue;
        }

        int b0 = b - b % 3;
        int al = a0 + (a + 1) % 3;
        int bl = b0 + (b + 2) % 3;

        unsigned int p0 = m_triangles[ar];
        unsigned int pr = m_triangles[a];
        unsigned int pl = m_triangles[al];
        unsigned int p1 = m_triangles[bl];

        bool illegal = inCircle(
            m_coords[2 * p0], m_coords[2 * p0 + 1],
            m_coords[2 * pr], m_coords[2 * pr + 1],
            m_coords[2 * pl], m_coords[2 * pl + 1],
            m_coords[2 * p1], m_coords[2 * p1 + 1]);

        if (illegal) {
            m_triangles[a] = p1;
            m_triangles[b] = p0;

            int hbl = m_halfEdges[bl];

            // edge swapped on the other side of the hull (rare); fix the halfedge reference
            if (hbl == -1) {
                int e = m_hullStart;
                do {
                    if (m_hullTri[e] == bl) {
                        m_hullTri[e] = a;
                        break;
                    }
                    e = m_hullPrev[e];
                } while (e != m_hullStart);
            }
            link(a, hbl);
            link(b, m_halfEdges[ar]);
            link(ar, bl);

            int br = b0 + (b + 1) % 3;

            // don't worry about hitting the cap: it can only happen on extremely degenerate input
            if (i < EDGE_STACK.size()) {
                EDGE_STACK[i++] = br;
            }
        }
        else {
            if (i == 0) break;
            a = EDGE_STACK[--i];
        }
    }

    return ar;
}
