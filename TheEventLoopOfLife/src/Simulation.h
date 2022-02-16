#pragma once

#include <SFML/Graphics.hpp>
#include "Grass.h"
#include "Sheep.h"
#include "Grid.h"
#include "World.h"
#include <random>

class Simulation
{
public:
	Simulation(uint32_t _screenWidth,
			   uint32_t _screenHeight,
			   uint32_t _columns,
			   uint32_t _rows,
			   uint32_t _borderWidth,
			   uint32_t _tileSize);
	~Simulation();

	bool update(float deltaTime);
	void draw(sf::RenderWindow& _window);

private:
	World* world;
	uint32_t screenWidth, screenHeight;
	uint32_t senseDecideFrequency = 10, senseDecideCounter = 0;
	// todo: move to agents
};

