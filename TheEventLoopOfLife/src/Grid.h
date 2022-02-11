#pragma once

#include <vector>

class Grid
{
public:
	static Grid* Instance();
	void createGrid(uint32_t _columns = 10,
		 uint32_t _rows = 10,
		 uint32_t _borderWidth = 1,
		 uint32_t _tileSize = 64);
	void update();
	uint32_t Columns();
	uint32_t Rows();
	uint32_t TileSize();
	uint32_t Size();

	Grid(Grid& other) = delete;
	void operator=(const Grid&) = delete;

protected:
	static Grid* instance;

private:
	Grid();
	~Grid();

	uint32_t columns, rows;
	uint32_t size;
	uint32_t borderWidth;
	uint32_t tileSize;
};

