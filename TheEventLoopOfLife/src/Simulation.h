// Simulation.h

#pragma once

#include <SFML/Graphics.hpp>

class Simulation
{
public:
	Simulation(unsigned int& screenWidth, unsigned int& screenHeight, const char*& title);
	~Simulation();

	bool Update(float deltaTime);
	void Draw(sf::RenderWindow& _window);


};

