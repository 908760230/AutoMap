#pragma once

#include <qvector.h>

class Delaunator
{
public:
	Delaunator(const QVector<QPoint>& data);
	Delaunator(const Delaunator& other);

	QVector<unsigned int> m_triangles;
	QVector<int> m_halfEdges;
private:
	void update();
	int hashKey(double x, double y);
	int addTriangle(int i0,int i1,int i2,int a,int b,int c);
	void link(int a, int b);
	int legalize(int a);
private:
	QVector<double> m_coords;

	// arrays that will store the triangulation graph

	// temporary arrays for tracking the edges of the advancing convex hull
	size_t m_hashSize = 0;
	int m_hullStart;
	int m_trianglesLen;
	QVector<unsigned int> m_hull;
	QVector<unsigned int> m_hullPrev;
	QVector<unsigned int> m_hullNext;
	QVector<unsigned int> m_hullTri;
	QVector<int> m_hullHash;

	// temporary arrays for sorting points
	QVector<unsigned int> m_ids;
	QVector<double> m_dists;

	double m_cx;
	double m_cy;
};

