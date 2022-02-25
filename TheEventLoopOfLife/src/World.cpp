#include "World.h"

World::World(uint32_t _columns,
			 uint32_t _rows,
			 uint32_t _screenWidth,
			 uint32_t _borderWidth,
			 uint32_t _tileSize)
{
	tileSize = _tileSize;
	grid = Grid::Instance();
	grid->createGrid(_columns, _rows, _borderWidth, _tileSize);

	int tileAmount = _columns * _rows;

	for (int i = 0; i < tileAmount; i++) {
		Grass* grass = new Grass();
		grass->setPos(
			i % _columns,
			floor(i / _rows));
		grass->create();
		if (rand() % 100 < grassSpawnChance) {
			grass->setState(GrassState::SEED);
			grass->setHealth(float(rand() / (RAND_MAX + 1.)));
		}
		if (i < grid->Columns()
			|| i % grid->Columns() == 0
			|| i % grid->Columns() == grid->Columns() - 1
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

	for (int i = 0; i < sheepAmount; i++) {
		sf::Vector2i spawnPos = sf::Vector2i{
			rand() % (int)_columns,
			rand() % (int)_rows };
		Sheep* sheep = new Sheep(spawnPos);
		for (int j = 0; j < grassArray.size(); ++j) {
			sheep->addObserver(grassArray[j]);
		}
		sheepArray.push_back(sheep);
		sheep->addObserver(this);
	}

	for (int i = 0; i < wolfAmount; i++) {
		sf::Vector2i spawnPos = sf::Vector2i{
			rand() % (int)_columns,
			rand() % (int)_rows };
		Wolf* wolf = new Wolf(spawnPos);
		for (int j = 0; j < grassArray.size(); ++j) {
			wolf->addObserver(grassArray[j]);
		}
		wolfArray.push_back(wolf);
		wolf->addObserver(this);
	}

	for (int i = 0; i < crowAmount; i++) {
		sf::Vector2i spawnPos = sf::Vector2i{
			rand() % (int)_columns,
			rand() % (int)_rows };
		Crow* crow = new Crow(spawnPos);
		crowArray.push_back(crow);
		crow->addObserver(this);
	}
}

World::~World()
{
	for (int i = 0; i < grassArray.size(); i++) {
		grassArray[i]->removeAll();
		delete grassArray[i];
		grassArray[i] = nullptr;
	}

	for (int i = 0; i < sheepArray.size(); i++) {
		sheepArray[i]->removeAll();
		delete sheepArray[i];
		sheepArray[i] = nullptr;
	}

	for (int i = 0; i < wolfArray.size(); i++) {
		wolfArray[i]->removeAll();
		delete wolfArray[i];
		wolfArray[i] = nullptr;
	}

	for (int i = 0; i < crowArray.size(); i++) {
		crowArray[i]->removeAll();
		delete crowArray[i];
		crowArray[i] = nullptr;
	}
}

void World::act()
{
	removeDeadAgents();
	plantGrass(1);

	for (auto grass = grassArray.begin(); grass != grassArray.end(); grass++) {
		if ((*grass)->isSensingDeciding()) {
			(*grass)->sense();
			(*grass)->decide();
		}
		(*grass)->act();
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); sheep++) {
		if ((*sheep)->isSensingDeciding()) {
			std::vector<sf::Vector2i*> wolfPositions;
			for (auto wolf = wolfArray.begin(); wolf != wolfArray.end(); ++wolf) {
				wolfPositions.push_back(&(*wolf)->pos);
			}
			(*sheep)->sense(grassArray, wolfPositions);
			(*sheep)->decide();
		}
		(*sheep)->act();
	}

	for (auto wolf = wolfArray.begin(); wolf != wolfArray.end(); wolf++) {
		if ((*wolf)->isSensingDeciding()) {
			(*wolf)->sense(sheepArray);
			(*wolf)->decide();
		}
		(*wolf)->act();
	}

	for (auto crow = crowArray.begin(); crow != crowArray.end(); crow++) {
		(*crow)->update(crowArray);
	}
}

void World::draw(sf::RenderWindow& _window)
{
	for (auto grass = grassArray.begin(); grass != grassArray.end(); ++grass) {
		//(*grass)->drawDebug();
		_window.draw((*grass)->getRect());
	}

	for (auto sheep = sheepArray.begin(); sheep != sheepArray.end(); ++sheep) {
		_window.draw((*sheep)->getBody());
	}

	for (auto wolf = wolfArray.begin(); wolf != wolfArray.end(); ++wolf) {
		_window.draw((*wolf)->getBody());
	}

	for (auto crow = crowArray.begin(); crow != crowArray.end(); ++crow) {
		_window.draw((*crow)->getBody());
		_window.draw((*crow)->getShadow());
	}
}

void World::onNotify(const Agent& _agent, Event _event)
{
	sf::Vector2i spawnPos;
	switch (_event) {
	case Event::DEATH_SHEEP:
		for (int i = 0; i < sheepArray.size(); ++i) {
			if (sheepArray[i]->id == _agent.id) {
				toDeleteType = ToDeleteType::SHEEP;
				toDeleteIndex = i;
				break;
			}
		}
		break;
	case Event::DEATH_WOLF:
		for (int i = 0; i < wolfArray.size(); ++i) {
			if (wolfArray[i]->id == _agent.id) {
				toDeleteType = ToDeleteType::WOLF;
				toDeleteIndex = i;
				break;
			}
		}
		break;
	case Event::BREED_SHEEP:
		spawnPos = grid->GetRandomNeighbor(_agent.pos);
		Sheep* sheep;
		sheep = new Sheep(spawnPos);
		for (int j = 0; j < grassArray.size(); ++j) {
			sheep->addObserver(grassArray[j]);
		}
		sheepToBreed.push_back(sheep);
		sheep->addObserver(this);
		break;

	case Event::BREED_WOLF:
		spawnPos = grid->GetRandomNeighbor(_agent.pos);
		Wolf* wolf = new Wolf(spawnPos);
		for (int j = 0; j < grassArray.size(); ++j) {
			wolf->addObserver(grassArray[j]);
		}
		wolfToBreed.push_back(wolf);
		wolf->addObserver(this);
		break;
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

void World::plantGrass(int _chance)
{
	int randomGrassIndex = rand() % grassArray.size();
	if (rand() % 100 < _chance) {
		grassArray[randomGrassIndex]->setState(GrassState::SEED);
		grassArray[randomGrassIndex]->setHealth(0.0f);
	}
}

void World::removeDeadAgents()
{	// note: I know that this way of discerning types is horrible practice.
	//	     I just can't figure out anything else at the moment.
	std::vector<Sheep*>::iterator sheepIterator;
	std::vector<Wolf*>::iterator wolfIterator;
	if (toDeleteIndex > -1) {
		switch (toDeleteType) {
		case ToDeleteType::GRASS:

			break;
		case ToDeleteType::SHEEP:
			sheepIterator = sheepArray.begin() + toDeleteIndex;
			delete sheepArray[toDeleteIndex];
			sheepArray[toDeleteIndex] = nullptr;
			sheepArray.erase(sheepIterator);
			break;
		case ToDeleteType::WOLF:
			wolfIterator = wolfArray.begin() + toDeleteIndex;
			delete wolfArray[toDeleteIndex];
			wolfArray[toDeleteIndex] = nullptr;
			wolfArray.erase(wolfIterator);
			break;
		}
		toDeleteIndex = -1;
	}

	sheepArray.insert(sheepArray.end(), sheepToBreed.begin(), sheepToBreed.end());
	sheepToBreed.clear();

	wolfArray.insert(wolfArray.end(), wolfToBreed.begin(), wolfToBreed.end());
	wolfToBreed.clear();
}
