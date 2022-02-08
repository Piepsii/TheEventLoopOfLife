// Simulation.cpp

#include "Simulation.h"

Simulation::Simulation(uint32_t _screenWidth, uint32_t _screenHeight, uint32_t _columns, uint32_t _rows)
	: screenWidth(_screenWidth)
	, screenHeight(_screenHeight)
{
	srand(time(0));
	world = new World(_columns, _rows, _screenWidth);
}

Simulation::~Simulation()
{
	for (int i = 0; i < sheepArray.size(); i++) {
		for (int j = 0; j < grassArray.size(); ++j) {
			sheepArray[i]->removeObserver(&grassArray[j]);
		}
		delete sheepArray[i];
		sheepArray[i] = nullptr;
	}
}

bool Simulation::Update(float deltaTime)
{
	if (senseDecideCounter == 0) {
		world->Sense();
		world->Decide();
	}
	world->Act();

	senseDecideCounter++;
	if (senseDecideCounter == senseDecideFrequency)
		senseDecideCounter = 0;

	//if (rand() % 100 < grassArray[i].spreadChance && grassArray[i].state == GrassState::Mature) {
	//	int spreadTo = grassArray[i].getRandomNeighborAsIndex();
	//	if (grassArray[spreadTo].state == GrassState::Dirt) { // move to grass
	//		grassArray[spreadTo].setState(GrassState::Seed);
	//	}
	//}

	return true;
}

void Simulation::Draw(sf::RenderWindow& _window)
{
	world->Draw(_window);
}
