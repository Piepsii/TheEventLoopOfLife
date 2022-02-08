#pragma once

#include <stdexcept>
#include <forward_list>

#include "Grass.h"
#include "Sheep.h"
#include "Grid.h"

class World
{
public:
	World(uint32_t _columns, uint32_t _rows, uint32_t _screenWidth);
	~World();

	void Sense();
	void Decide();
	void Act();
	void Draw(sf::RenderWindow& _window);

private:
	std::vector<Grass*> getNeighboringGrasses(int index);
	Grass* getGrassAtPos(uint32_t x, uint32_t y);

	Grid grid;
	std::vector<Grass*> grassArray;
	std::vector<Sheep*> sheepArray;
	uint32_t grassSpawnChance = 30;
	float tileSize = 1.0f;
};

