// Simulation.cpp

#include "Simulation.h"

Simulation::Simulation(unsigned int _screenWidth, unsigned int _screenHeight, unsigned int _gridSize)
{
	srand(time(0));
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	gridSize = _gridSize;

	tileSize = float(screenWidth) / float(gridSize);
	int tileAmount = gridSize * gridSize;
;
	for (int i = 0; i < tileAmount; i++) {
		Grass grass = Grass();
		grass.setSize(tileSize);
		grass.setPos(
			i % gridSize,
			floor(i / gridSize));
		grass.Create();
		if (rand() % 100 < (int) grassSpawnChance) {
			grass.setState(GrassState::Seed);
			grass.setHealth(0.01f);
		}
		grassArray.push_back(grass);
	}

	for (int i = 0; i < 1 /*(int)sheepAmount*/; i++) {
		Sheep* sheep = new Sheep();
		float sheepSize = tileSize * 0.3f;
		sheep->setSize(sheepSize);
		sf::Vector2i pos = sf::Vector2i(
			rand() % gridSize,
			rand() % gridSize);
		sheep->setPos(pos);
		sheep->Create(tileSize);
		sheepArray.push_back(sheep);
	}
	sheepArray[0]->debug = true;
}

Simulation::~Simulation()
{
}

bool Simulation::Update(float deltaTime)
{
	tileSize = float(screenWidth) / float(gridSize);

	for (int i = 0; i < gridSize * gridSize; i++) {
		if (senseDecideCounter == 0) {
			grassArray[i].Sense();
			grassArray[i].Decide();
		}

		grassArray[i].Act();
		if (rand() % 100 < grassArray[i].spreadChance && grassArray[i].state == GrassState::Mature) {
			int spreadTo = grassArray[i].getRandomNeighborAsIndex();
			if(grassArray[spreadTo].state == GrassState::Dirt )
				grassArray[spreadTo].setState(GrassState::Seed);
		}
	}

	for (int i = 0; i < sheepArray.size(); i++) {
		if (senseDecideCounter == 0) {
			sheepArray[i]->Sense();
			sheepArray[i]->Decide();
		}

		sheepArray[i]->Act(grassArray);
		sheepArray[i]->updateShape(tileSize);
	}

	senseDecideCounter++;
	if (senseDecideCounter == senseDecideFrequency)
		senseDecideCounter = 0;

	return true;
}

void Simulation::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < grassArray.size(); i++) {
		grassArray[i].DrawDebug();
		_window.draw(grassArray[i].getRect());
	}

	for (int i = 0; i < sheepArray.size(); i++) {
		_window.draw(sheepArray[i]->getBody());
		_window.draw(sheepArray[i]->getHead());
	}
}
