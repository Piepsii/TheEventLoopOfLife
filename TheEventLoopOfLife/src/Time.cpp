#include "Time.h"

float Time::time = 0.0f;
float Time::deltaTime = 0.0f;
float Time::timeScale = 1.0f;
sf::Clock Time::simClock;
sf::Clock Time::deltaClock;

bool Time::update()
{
	time = simClock.getElapsedTime().asSeconds() * timeScale;
	deltaTime = deltaClock.getElapsedTime().asSeconds();
	deltaClock.restart();

	return true;
}
