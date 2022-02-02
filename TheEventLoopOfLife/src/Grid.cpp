#include "Grid.h"

Grid::Grid(unsigned int _columns, unsigned int _rows,unsigned int _borderWidth, unsigned int _tileSize)
	:  columns(_columns)
	, rows(_rows)
	, borderWidth(_borderWidth)
	, tileSize(_tileSize)
{
}

Grid::~Grid()
{
}

void Grid::Update()
{
}

unsigned int Grid::Columns()
{
	return columns;
}

unsigned int Grid::Rows()
{
	return rows;
}

unsigned int Grid::TileSize()
{
	return tileSize;
}
