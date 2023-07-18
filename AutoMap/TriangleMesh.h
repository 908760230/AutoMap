#pragma once
#include <qvector.h>
#include "Delaunator.h"

/**
 * Each initial point generates one region, where
 * points.slice(0, numBoundaryPoints) are considered to be
 * boundary points/regions.
 *
 * As created from Delaunator, the mesh has some sides without pairs.
 * Optionally use TriangleMesh.addGhostStructure() to add "ghost"
 * sides, triangles, and region to complete the mesh. Elements that
 * aren't "ghost" are called "solid".
 */
struct MeshInitializer
{
	QVector<QPointF> points;
	Delaunator delaunator;
	int numBoundaryPoints;
	int numSolidSides;
};

/**
 * Represent a triangle-polygon dual mesh with:
 *   - Regions (r)
 *   - Sides (s)
 *   - Triangles (t)
 *
 * Each element has an id:
 *   - 0 <= r < numRegions
 *   - 0 <= s < numSides
 *   - 0 <= t < numTriangles
 *
 * Naming convention: y_name_x takes x (r, s, t) as input and produces
 * y (r, s, t) as output.
 *
 * A side is directed. If two triangles t0, t1 are adjacent, there will
 * be two sides representing the boundary, one for t0 and one for t1. These
 * can be accessed with t_inner_s and t_outer_s.
 *
 * A side also represents the boundary between two regions. If two regions
 * r0, r1 are adjacent, there will be two sides representing the boundary,
 * r_begin_s and r_end_s.
 *
 * A side from p-->q will have a pair q-->p, at index
 * s_opposite_s. It will be -1 if the side doesn't have a pair.
 * Use addGhostStructure() to add ghost pairs to all sides.
 */
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
	QVector<unsigned char> is_boundary_t;
	QVector<double> length_s;
	QVector<int> s_of_r;

	TriangleMesh(const MeshInitializer &init);

	static MeshInitializer addGhostStructure(QVector<QPointF> &points, const Delaunator &delaunator, int numBoundaryPoints);
	bool is_ghost_s(int s){ return s >= numSolidSides; }
	QVector<unsigned int> r_around_t(int t);
	bool is_boundary_r(int r){ return r < numBoundaryRegions; }
	unsigned int r_begin_s(unsigned int s) { return m_triangles[s]; }
	unsigned int r_end_s(unsigned int s){ return m_triangles[s_next_s(s)]; }
	double x_of_r(unsigned int r) const { return m_vertex_r[r].x(); }
	double y_of_r(unsigned int r ) const { return m_vertex_r[r].y(); }
	double x_of_t(unsigned int t) const { return m_vertex_t[t].x(); }
	double y_of_t(unsigned int t) const { return m_vertex_t[t].y(); }
	int t_inner_s(int s){ return t_from_s(s); }
private:
	void update();
	static int s_next_s(int s){ return (s % 3 == 2) ? s - 2 : s + 1; }
	static int t_from_s(int s){ return (s / 3) | 0; }
private:
	QVector<int> m_halfedges;
	QVector<unsigned int> m_triangles;
	QVector<QPointF> m_vertex_t;
	QVector<QPointF> m_vertex_r;
	//_options: any; // any other information we need to carry

};

