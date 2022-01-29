// Simulation.h

#pragma once

#include <SFML/Graphics.hpp>
#include "Grass.h"
#include "Sheep.h"
#include <random>

class Simulation
{
public:
	Simulation(unsigned int _screenWidth, unsigned int _screenHeight, unsigned int _gridSize);
	~Simulation();

	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& _window);

private:
	std::vector<Grass> grassArray;
	std::vector<Sheep*> sheepArray;
	unsigned int sheepAmount = 10;
	unsigned int screenWidth, screenHeight;
	unsigned int gridSize, tileSize;
	unsigned int grassSpawnChance = 30;
	unsigned int senseDecideFrequency = 10, senseDecideCounter = 0;
};

