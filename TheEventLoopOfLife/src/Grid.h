#pragma once

#include <vector>
#include "Sheep.h"
#include "Grass.h"

class Grid
{
public:
	Grid(unsigned int _columns = 10, unsigned int _rows = 10, unsigned int _borderWidth = 1, unsigned int _tileSize = 64);
	~Grid();

	void Update();

	unsigned int Columns();
	unsigned int Rows();
	unsigned int TileSize();

private:
	unsigned int columns, rows;
	unsigned int borderWidth;
	unsigned int tileSize;
};

