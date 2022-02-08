#include "World.h"

World::World(uint32_t _columns, uint32_t _rows, uint32_t _screenWidth)
{
	tileSize = float(_screenWidth) / float(grid.Columns());
	grid = Grid(_columns, _rows, 1, tileSize);

	int tileAmount = _columns * _rows;

	for (int i = 0; i < tileAmount; i++) {
		Grass* grass = new Grass();
		grass->setSize(tileSize);
		grass->setPos(
			i % _columns,
			floor(i / _rows));
		grass->Create();
		if (rand() % 100 < (int)grassSpawnChance) {
			grass->setState(GrassState::Seed);
			grass->setHealth(0.01f);
		}
		grassArray.push_back(grass);
	}

	for (int i = 0; i < 1 /*(int)sheepAmount*/; i++) {
		Sheep* sheep = new Sheep();
		float sheepSize = tileSize * 0.3f;
		sheep->setSize(sheepSize);
		sheep->setPos(
			rand() % _columns,
			rand() % _rows);
		sheep->Create(tileSize);
		for (int j = 0; j < grassArray.size(); ++j) {
			sheep->addObserver(grassArray[j]);
		}
		sheepArray.push_back(sheep);
	}
	sheepArray[0]->debug = true;
}

void World::Sense()
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->Sense();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->Sense();
	}
}

void World::Decide()
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->Decide();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->Decide();
	}
}

void World::Act()
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->Act();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->Act(grassArray);
	}
}

void World::Draw(sf::RenderWindow& _window)
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->DrawDebug();
		_window.draw((*grass)->getRect());
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->updateShape(tileSize);
		_window.draw((*sheep)->getBody());
		_window.draw((*sheep)->getHead());
	}
}
