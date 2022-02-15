#include "Grid.h"

Grid* Grid::instance = nullptr;

Grid* Grid::Instance()
{
	if (instance == nullptr) {
		instance = new Grid();
	}
	return instance;
}

void Grid::createGrid(uint32_t _columns,
					  uint32_t _rows,
					  uint32_t _borderWidth,
					  uint32_t _tileSize)
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

sf::Vector2i Grid::GetRandomNeighbor(sf::Vector2i _pos)
{
	int neighborX = -1, neighborY = -1;
	while (neighborX < 0 || neighborX >= columns) {
		int offsetX = rand() % 3 - 1;
		neighborX = _pos.x + offsetX;
	}
	while (neighborY < 0 || neighborY >= rows) {
		int offsetY = rand() % 3 - 1;
		neighborY = _pos.y + offsetY;
	}
	sf::Vector2i result = { neighborX, neighborY };
	return result;
}

uint32_t Grid::GetRandomNeighborAsIndex(sf::Vector2i _pos)
{
	int neighborX = -1, neighborY = -1;
	while (neighborX < 0 || neighborX >= columns) {
		int offsetX = rand() % 3 - 1;
		neighborX = _pos.x + offsetX;
	}
	while (neighborY < 0 || neighborY >= rows) {
		int offsetY = rand() % 3 - 1;
		neighborY = _pos.y + offsetY;
	}

	uint32_t index = neighborY * columns + neighborX;
	return index;
}

Grid::Grid() 
{

}

Grid::~Grid()
{
	delete instance;
	instance = nullptr;
}
