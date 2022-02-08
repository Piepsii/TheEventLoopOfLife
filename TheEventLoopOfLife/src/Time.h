#pragma once
#include <SFML/System.hpp>

class Time
{
public:
	static bool Update();

	static float time, deltaTime;
	static float timeScale;

private:
	static sf::Clock simClock, deltaClock;
};

