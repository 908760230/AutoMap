#pragma once
#include <qvector.h>
#include "Delaunator.h"

struct MeshInitializer
{
	QVector<QPoint> points;
	Delaunator delaunator;
	int numBoundaryPoints;
	int numSolidSides;
};

class TriangleMesh
{
public:
	unsigned int numSides;
	unsigned int numSolidSides;
	unsigned int numRegions;
	unsigned int numSolidRegions;
	unsigned int numTriangles;
	unsigned int numSolidTriangles;
	unsigned int numBoundaryRegions;

	TriangleMesh(const MeshInitializer &init);

	static MeshInitializer addGhostStructure(QVector<QPoint> &points, const Delaunator &delaunator, int numBoundaryPoints);
private:
	void update();
	static int s_next_s(int s){ return (s % 3 == 2) ? s - 2 : s + 1; }
private:
	QVector<int> m_halfedges;
	QVector<unsigned int> m_triangles;
	QVector<int> m_s_of_r;
	QVector<QPointF> m_vertex_t;
	QVector<QPoint> m_vertex_r;

	//_options: any; // any other information we need to carry

};

