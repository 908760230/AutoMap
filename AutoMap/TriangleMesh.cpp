#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const MeshInitializer& init)
{
	this->numBoundaryRegions = init.numBoundaryPoints ? init.numBoundaryPoints : 0;
	this->numSolidSides = init.numSolidSides ? init.numSolidSides : 0;
	m_vertex_r = init.points;
	m_triangles = init.delaunator.m_triangles;
	m_halfedges = init.delaunator.m_halfEdges;

	update();
}

MeshInitializer TriangleMesh::addGhostStructure(QVector<QPointF>& points,const Delaunator& delaunator, int numBoundaryPoints)
{
    int numSolidSides = delaunator.m_triangles.size();

    int numUnpairedSides = 0, firstUnpairedEdge = -1;
    QVector<unsigned int> s_unpaired_r(numSolidSides); // seed to side
    for (unsigned int s = 0; s < numSolidSides; s++) {
        if (delaunator.m_halfEdges[s] == -1) {
            numUnpairedSides++;
            s_unpaired_r[delaunator.m_triangles[s]] = s;
            firstUnpairedEdge = s;
        }
    }

    unsigned int r_ghost = points.size();
    QVector<QPointF> newpoints = points;
    newpoints.push_back(QPointF(std::numeric_limits<int>::quiet_NaN(), std::numeric_limits<int>::quiet_NaN()));
    QVector<int> r_newstart_s(numSolidSides + 3 * numUnpairedSides, 0);
    for(unsigned int index =0; index < delaunator.m_triangles.size();index++)
        r_newstart_s[index] = delaunator.m_triangles[index];
    QVector<int> s_newopposite_s(numSolidSides + 3 * numUnpairedSides, 0);
    for (unsigned int index = 0; index < delaunator.m_halfEdges.size(); index++)
        s_newopposite_s[index] = delaunator.m_halfEdges[index];

    for (unsigned int i = 0, s = firstUnpairedEdge;
        i < numUnpairedSides;
        i++, s = s_unpaired_r[r_newstart_s[s_next_s(s)]]) {

        // Construct a ghost side for s
        unsigned int s_ghost = numSolidSides + 3 * i;
        s_newopposite_s[s] = s_ghost;
        s_newopposite_s[s_ghost] = s;
        r_newstart_s[s_ghost] = r_newstart_s[s_next_s(s)];

        // Construct the rest of the ghost triangle
        r_newstart_s[s_ghost + 1] = r_newstart_s[s];
        r_newstart_s[s_ghost + 2] = r_ghost;
        unsigned int k = numSolidSides + (3 * i + 4) % (3 * numUnpairedSides);
        s_newopposite_s[s_ghost + 2] = k;
        s_newopposite_s[k] = s_ghost + 2;
    }

    return {
        newpoints,
        delaunator,
        numBoundaryPoints,
        numSolidSides,
    };
}

void TriangleMesh::update()
{
    numSides = m_triangles.size();
    numRegions = m_vertex_r.size();
    numSolidRegions = numRegions - 1; // TODO: only if there are ghosts
    numTriangles = numSides / 3;
    numSolidTriangles = numSolidSides / 3;

    if (m_vertex_t.size() < numTriangles) {
        // Extend this array to be big enough
        int numOldTriangles = m_vertex_t.size();
        int numNewTriangles = numTriangles - numOldTriangles;
        QVector<QPointF> tmpVector(m_vertex_t.size() + numNewTriangles, {0,0});
        for (int t = 0; t < numOldTriangles; t++) {
            tmpVector[t] = m_vertex_t[t];
        }
    }

    // Construct an index for finding sides connected to a region
    m_s_of_r.resize(numRegions);
    for (int s = 0; s < m_triangles.size(); s++) {
        int endpoint = m_triangles[s_next_s(s)];
        if (m_s_of_r[endpoint] == 0 || m_halfedges[s] == -1) {
            m_s_of_r[endpoint] = s;
        }
    }

    // Construct triangle coordinates
    for (int s = 0; s < m_triangles.size(); s += 3) {
        int t = s / 3;
        auto a = m_vertex_r[m_triangles[s]];
        auto b = m_vertex_r[m_triangles[s + 1]];
        auto c = m_vertex_r[m_triangles[s + 2]];
        if (is_ghost_s(s)) {
            // ghost triangle center is just outside the unpaired side
            double dx = b.x() - a.x(), dy = b.y() - a.y();
            double scale = 10.0 / sqrtf(dx * dx + dy * dy); // go 10units away from side
            m_vertex_t[t].setX(0.5 * (a.x() + b.x()) + dy * scale);
            m_vertex_t[t].setY(0.5 * (a.y() + b.y()) - dx * scale);
        }
        else {
            // solid triangle center is at the centroid
            m_vertex_t[t].setX((a.x() + b.x() + c.x()) / 3);
            m_vertex_t[t].setY((a.y() + b.y() + c.y()) / 3);
        }
    }
}
