// Simulation.cpp

#include "Simulation.h"

Simulation::Simulation(unsigned int _screenWidth, unsigned int _screenHeight, unsigned int _gridSize)
{
	screenWidth = _screenWidth;
	screenHeight = _screenHeight;
	gridSize = _gridSize;

	const float tileSize = screenWidth / gridSize;
	int tileAmount = gridSize * gridSize;

	grassArray = new Grass[tileAmount];
	for (int i = 0; i < tileAmount; i++) {
		grassArray[i].setSize(tileSize);
		grassArray[i].setIndex(i);
		grassArray[i].Create();
	}
}

Simulation::~Simulation()
{
}

bool Simulation::Update(float deltaTime)
{
	return true;
}

void Simulation::Draw(sf::RenderWindow& _window)
{
	for (int i = 0; i < gridSize * gridSize; i++) {
		_window.draw(grassArray[i].getRect());
	}
}
