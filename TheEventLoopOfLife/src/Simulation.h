// Simulation.h

#pragma once

#include <SFML/Graphics.hpp>
#include "Grass.h"

class Simulation
{
public:
	Simulation(unsigned int _screenWidth, unsigned int _screenHeight, unsigned int _gridSize);
	~Simulation();

	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& _window);

private:
	unsigned int screenWidth, screenHeight;
	unsigned int gridSize;
	Grass* grassArray;

};

