#include "Simulation.h"

Simulation::Simulation(uint32_t _screenWidth,
					   uint32_t _screenHeight,
					   uint32_t _columns,
					   uint32_t _rows,
					   uint32_t _borderWidth,
					   uint32_t _tileSize)
	: screenWidth(_screenWidth)
	, screenHeight(_screenHeight)
{
	srand((unsigned int)time(0));
	world = new World(_columns, _rows, _screenWidth, _borderWidth, _tileSize);
}

Simulation::~Simulation()
{
	delete world;
	world = nullptr;
}

bool Simulation::update()
{
	world->act();
	return true;
}

void Simulation::draw(sf::RenderWindow& _window)
{
	world->draw(_window);
}
