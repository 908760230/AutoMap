#pragma once

#include <qthread.h>
#include <qvector.h>

class MapGenerator {
public:
	MapGenerator(int size);
	~MapGenerator() {};

	void setSeedAndIsland(int seed, int island);
	void generate();
private:

private:
	int m_seed = 0;
	int m_island = 0;
	bool m_userHasPainted = false;
	QVector<double> m_elevation;
};