#pragma once

#include <SFML/Graphics.hpp>
#include "Grass.h"
#include "Sheep.h"
#include "Grid.h"
#include <random>

class Simulation
{
public:
	Simulation(unsigned int _screenWidth, unsigned int _screenHeight);
	~Simulation();

	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& _window);

	void createGrid(unsigned int _columns, unsigned int _rows);

private:
	Grid grid;
	std::vector<Grass> grassArray;
	std::vector<Sheep*> sheepArray;
	unsigned int sheepAmount = 10;
	unsigned int screenWidth, screenHeight;
	unsigned int grassSpawnChance = 30;
	unsigned int senseDecideFrequency = 10, senseDecideCounter = 0;
};

