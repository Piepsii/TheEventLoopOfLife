// Simulation.cpp

#include "Simulation.h"

Simulation::Simulation(unsigned int _screenWidth, unsigned int _screenHeight, unsigned int _gridSize)
{
	srand(time(NULL));
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	gridSize = _gridSize;

	const float tileSize = screenWidth / gridSize;
	int tileAmount = gridSize * gridSize;

	grassArray = new Grass[tileAmount];
	for (int i = 0; i < tileAmount; i++) {
		grassArray[i].setSize(tileSize);
		grassArray[i].setPos((i % gridSize), floor(i / gridSize));
		grassArray[i].Create();
		if (rand() % 100 < grassSpawnChance) {
			grassArray[i].setState(GrassState::Seed);
		}
	}
}

Simulation::~Simulation()
{
}

bool Simulation::Update(float deltaTime)
{
	for (int i = 0; i < gridSize * gridSize; i++) {
		grassArray[i].Update();
		if (rand() % 100 < grassArray[i].spreadChance && grassArray[i].state == GrassState::Mature) {
			int spreadTo = grassArray[i].getRandomNeighborAsIndex();
			if(grassArray[spreadTo].state ==GrassState::Dirt )
				grassArray[spreadTo].setState(GrassState::Seed);
		}
	}
	return true;
}

void Simulation::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < gridSize * gridSize; i++) {
		_window.draw(grassArray[i].getRect());
	}
}
