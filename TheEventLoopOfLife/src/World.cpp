#include "World.h"

World::World(uint32_t _columns, uint32_t _rows, uint32_t _screenWidth)
{
	tileSize = float(_screenWidth) / float(_columns);
	grid = Grid::Instance();
	grid->createGrid(_columns, _rows, 1, tileSize);

	int tileAmount = _columns * _rows;

	for (int i = 0; i < tileAmount; i++) {
		Grass* grass = new Grass();
		grass->setSize(tileSize);
		grass->setPos(
			i % _columns,
			floor(i / _rows));
		grass->create();
		if (rand() % 100 < (int)grassSpawnChance) {
			grass->setState(GrassState::Seed);
			grass->setHealth(float(rand() / (RAND_MAX + 1.)));
		}
		if (i < grid->Columns()
			|| i % grid->Columns() - 1 == 0
			|| i % grid->Columns() == 0
			|| i >(grid->Rows() - 1) * grid->Columns()) {
			grass->gridState = GridState::EDGE;
		}
		if (i == 0
			|| i == grid->Columns() - 1
			|| i == (grid->Rows() - 1) * grid->Columns()
			|| i == grid->Rows() * grid->Columns() - 1) {
			grass->gridState = GridState::CORNER;
		}
		grassArray.push_back(grass);
		grass->addObserver(this);
	}

	for (int i = 0; i < grassArray.size(); ++i) {
		auto neighbors = getNeighboringGrasses(i);
		for (auto neighbor = neighbors.begin(); neighbor != neighbors.end(); ++neighbor)
			grassArray[i]->addObserver(*neighbor);
	}

	for (int i = 0; i < tileAmount / 10; i++) {
		Sheep* sheep = new Sheep();
		float sheepSize = tileSize * 0.3f;
		sheep->setSize(sheepSize);
		sheep->setPos(
			rand() % _columns,
			rand() % _rows);
		sheep->create(tileSize);
		for (int j = 0; j < grassArray.size(); ++j) {
			sheep->addObserver(grassArray[j]);
		}
		sheepArray.push_back(sheep);
		sheep->addObserver(this);
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

void World::sense()
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->sense();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->sense(grassArray);
	}
}

void World::decide()
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->decide();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->decide();
	}
}

void World::act()
{
	// note: I know that this way of discerning types is horrible practice.
	//	     I just can't figure out anything else at the moment.
	if (toDeleteIndex > -1) {
		switch (toDeleteType) {
		case ToDeleteType::GRASS:

			break;
		case ToDeleteType::SHEEP:
			auto corpse = sheepArray.begin() + toDeleteIndex;
			delete sheepArray[toDeleteIndex];
			sheepArray[toDeleteIndex] = nullptr;
			sheepArray.erase(corpse);
			break;
		}
		toDeleteIndex = -1;
	}

	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->act();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->act(grassArray);
	}
}

void World::draw(sf::RenderWindow& _window)
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		(*grass)->drawDebug();
		_window.draw((*grass)->getRect());
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		(*sheep)->updateShape(tileSize);
		_window.draw((*sheep)->getBody());
		_window.draw((*sheep)->getHead());
	}
}

void World::onNotify(const Agent& _agent, Event _event)
{
	switch (_event) {
	case Event::DEATH:
		for (int i = 0; i < sheepArray.size(); ++i) {
			if (sheepArray[i]->pos == _agent.pos) {
				toDeleteType = ToDeleteType::SHEEP;
				toDeleteIndex = i;
				break;
			}
		}
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
	if (x < 0 || x >= grid->Columns() || y < 0 || y >= grid->Rows()) {
		return nullptr;
	}
	return grassArray[y * grid->Columns() + x];
}
