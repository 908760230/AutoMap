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

MeshInitializer TriangleMesh::addGhostStructure(QVector<QPoint>& points,const Delaunator& delaunator, int numBoundaryPoints)
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
    QVector<QPoint> newpoints = points;
    newpoints.push_back(QPoint(std::numeric_limits<int>::quiet_NaN(), std::numeric_limits<int>::quiet_NaN()));
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
}