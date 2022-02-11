#include "Grid.h"

Grid* Grid::instance = nullptr;

Grid* Grid::Instance()
{
	if (instance == nullptr) {
		instance = new Grid();
	}
	return instance;
}

void Grid::createGrid(uint32_t _columns, uint32_t _rows, uint32_t _borderWidth, uint32_t _tileSize)
{
	columns = _columns;
	rows = _rows;
	borderWidth = _borderWidth;
	tileSize = _tileSize;
}

void Grid::update()
{
}

uint32_t Grid::Columns()
{
	return columns;
}

uint32_t Grid::Rows()
{
	return rows;
}

uint32_t Grid::TileSize()
{
	return tileSize;
}

uint32_t Grid::Size()
{
	return columns * rows;
}

Grid::Grid() 
{

}

Grid::~Grid()
{
	delete instance;
	instance = nullptr;
}
