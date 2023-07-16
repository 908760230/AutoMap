#include "MapGenerator.h"

MapGenerator::MapGenerator(int size)
{
	m_elevation.resize(size * size);
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
}
