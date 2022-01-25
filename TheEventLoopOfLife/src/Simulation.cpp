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

	grassArray = new Grass[tileAmount];
	for (int i = 0; i < tileAmount; i++) {
		grassArray[i].setSize(tileSize);
		grassArray[i].setPos(
			i % gridSize,
			floor(i / gridSize));
		grassArray[i].Create();
		if (rand() % 100 < (int) grassSpawnChance) {
			grassArray[i].setState(GrassState::Seed);
			grassArray[i].setHealth(0.01f);
		}
	}

	sheepArray = new Sheep[sheepAmount];
	for (int i = 0; i < (int) sheepAmount; i++) {
		float sheepSize = tileSize * 0.3f;
		sheepArray[i].setSize(sheepSize);
		sf::Vector2i pos = sf::Vector2i(
			rand() % gridSize,
			rand() % gridSize);
		sheepArray[i].setPos(pos);
		sheepArray[i].Create(tileSize);
	}
}

Simulation::~Simulation()
{
	delete[] grassArray;
	grassArray = nullptr;
	delete[] sheepArray;
	sheepArray = nullptr;
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
			if(grassArray[spreadTo].state ==GrassState::Dirt )
				grassArray[spreadTo].setState(GrassState::Seed);
		}
	}

	for (int i = 0; i < sheepAmount; i++) {
		if (senseDecideCounter == 0) {
			sheepArray[i].Sense();
			sheepArray[i].Decide();
		}

		sheepArray[i].Act();
		sheepArray[i].updateShape(tileSize);
	}

	senseDecideCounter++;
	if (senseDecideCounter == senseDecideFrequency)
		senseDecideCounter = 0;

	return true;
}

void Simulation::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < gridSize * gridSize; i++) {
		_window.draw(grassArray[i].getRect());
	}

	for (int i = 0; i < sheepAmount; i++) {
		_window.draw(sheepArray[i].getCircle());
	}
}
