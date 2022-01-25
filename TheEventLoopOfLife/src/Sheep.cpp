#include "Sheep.h"

Sheep::Sheep()
{
}

void Sheep::Create(float _tileSize)
{
	circle = sf::CircleShape();
	circle.setRadius(size);
	circle.setOutlineColor(sf::Color::Blue);
	circle.setOrigin(size, size);
	circle.setPosition((_tileSize + 1) * pos.x + _tileSize / 2,
					   (_tileSize + 1) * pos.y + _tileSize / 2);
	circle.setFillColor(sf::Color::White);
}

void Sheep::Sense()
{
}

void Sheep::Decide()
{
}

void Sheep::Act()
{
	Age();
	if (health <= 0.0f) {
		Die();
	}
}

sf::CircleShape Sheep::getCircle()
{
	return circle;
}

void Sheep::Age()
{
	health -= 0.01f;
}

void Sheep::Die()
{

}
