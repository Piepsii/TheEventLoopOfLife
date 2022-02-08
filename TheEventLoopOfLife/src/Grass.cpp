#include "Grass.h"

Grass::Grass()
{
	seed = sf::Color(221, 255, 153, 255);
	mature = sf::Color(38, 77, 0, 255);
	dirt = sf::Color(77, 38, 0, 255);
}

void Grass::Create()
{
	rect = sf::RectangleShape(sf::Vector2f(size, size));
	rect.setPosition((size + 1) * pos.x, (size + 1) * pos.y);
	rect.setFillColor(dirt);
	rect.setOutlineThickness(1.0f);
}

void Grass::Sense()
{
}

void Grass::Decide()
{
	if (health >= 1.0f)
		setState(GrassState::Mature);

	if (health <= 0.0f)
		setState(GrassState::Dirt);

	if (wasTrampled) {
		setState(GrassState::Dirt);
		health = 0.0f;
	}
}

void Grass::Act()
{
	switch (state) {
	case GrassState::Seed:
		health < 1.0f ? health += growthFactor : health = health;
		rect.setFillColor(LerpRGB(seed, mature, health));

		break;
	case GrassState::Mature:
		health > 0.0f ? health -= witherFactor : health = health;
		rect.setFillColor(LerpRGB(mature, dirt, (1.0f - health)));

		break;
	case GrassState::Dirt:
		rect.setFillColor(dirt);
		break;
	}
}

void Grass::DrawDebug()
{
	if (marked) {
		rect.setOutlineColor(sf::Color::Red);
		rect.setOutlineThickness(2.0f);
	}
	else {
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(0.0f);
	}
	marked = false;
}

sf::RectangleShape Grass::getRect()
{
	return rect;
}

void Grass::setState(GrassState _state)
{
	state = _state;
}

void Grass::setHealth(float _health)
{
	health = _health;
}

int Grass::getRandomNeighborAsIndex()
{
	int neighborX = -1, neighborY = -1;
	while (neighborX < 0 || neighborX > 9) {
		int offsetX = rand() % 3 - 1;
		neighborX = pos.x + offsetX;
	}
	while (neighborY < 0 || neighborY > 9) {
		int offsetY = rand() % 3 - 1;
		neighborY = pos.y + offsetY;
	}

	int index = neighborX * 10 + neighborY;
	return index;
}

void Grass::onNotify(const Agent& _agent, Event _event) {
	switch (_event) {
	case Event::EVENT_TRAMPLE:
		if (_agent.pos == pos && state != GrassState::Dirt) {
			wasTrampled = true;
		}
		break;
	}
}
