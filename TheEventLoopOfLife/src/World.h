#pragma once

#include <stdexcept>
#include <forward_list>

#include "Grass.h"
#include "Sheep.h"
#include "Wolf.h"
#include "Crow.h"
#include "Grid.h"

enum class ToDeleteType {
	GRASS,
	SHEEP
};

class World : public Observer
{
public:
	World(uint32_t _columns,
		  uint32_t _rows,
		  uint32_t _screenWidth,
		  uint32_t _borderWidth,
		  uint32_t _tileSize);
	~World();

	void sense();
	void decide();
	void act();
	void draw(sf::RenderWindow& _window);
	void onNotify(const Agent& _agent, Event _event);

private:
	std::vector<Grass*> getNeighboringGrasses(int index);
	Grass* getGrassAtPos(uint32_t x, uint32_t y);

	Grid* grid;
	int sheepAmount = 20;
	int wolfAmount = 5;
	int crowAmount = 30;
	std::vector<Grass*> grassArray;
	std::vector<Sheep*> sheepArray;
	std::vector<Wolf*> wolfArray;
	std::vector<Crow*> crowArray;
	uint32_t grassSpawnChance = 30;
	float tileSize = 1.0f;

	ToDeleteType toDeleteType;
	int toDeleteIndex;
	std::vector<Sheep*> sheepToBreed;
	std::vector<Wolf*> wolfToBreed;
};

