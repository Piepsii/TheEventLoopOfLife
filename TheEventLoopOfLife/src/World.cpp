#include "World.h"

World::World(uint32_t _columns, uint32_t _rows, uint32_t _screenWidth)
{
	tileSize = float(_screenWidth) / float(_columns);
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
		if (i < grid.Columns()
			|| i % grid.Columns() - 1 == 0
			|| i % grid.Columns() == 0
			|| i >(grid.Rows() - 1) * grid.Columns()) {
			grass->gridState = GridState::EDGE;
		}
		if (i == 0
			|| i == grid.Columns() - 1
			|| i == (grid.Rows() - 1) * grid.Columns()
			|| i == grid.Rows() * grid.Columns() - 1) {
			grass->gridState = GridState::CORNER;
		}
		grassArray.push_back(grass);
	}

	for (int i = 0; i < grassArray.size(); ++i) {
		auto neighbors = getNeighboringGrasses(i);
		for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); ++neighbor)
			grassArray[i]->addObserver(*neighbor);
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

World::~World()
{
	for (int i = 0; i < sheepArray.size(); i++) {
		sheepArray[i]->removeAll();
		delete sheepArray[i];
		sheepArray[i] = nullptr;
	}

	for (int i = 0; i < grassArray.size(); i++) {
		grassArray[i]->removeAll();
		delete grassArray[i];
		grassArray[i] = nullptr;
	}
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

std::vector<Grass*> World::getNeighboringGrasses(int index)
{
	Grass* grass = grassArray[index];
	std::forward_list<Grass*> list;
	int x = grass->pos.x;
	int y = grass->pos.y;
	list.push_front(getGrassAtPos(x - 1, y - 1));
	list.push_front(getGrassAtPos(x, y - 1));
	list.push_front(getGrassAtPos(x + 1, y - 1));
	list.push_front(getGrassAtPos(x - 1, y));
	list.push_front(getGrassAtPos(x + 1, y));
	list.push_front(getGrassAtPos(x - 1, y + 1));
	list.push_front(getGrassAtPos(x, y + 1));
	list.push_front(getGrassAtPos(x + 1, y + 1));

	list.remove(nullptr);
	std::vector<Grass*> result{ std::begin(list), std::end(list) };
	return result;
}

Grass* World::getGrassAtPos(uint32_t x, uint32_t y)
{
	if (x < 0 || x >= grid.Columns() || y < 0 || y >= grid.Rows()) {
		return nullptr;
	}
	return grassArray[y * grid.Columns() + x];
}
