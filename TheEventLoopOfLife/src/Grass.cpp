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
	rect.setPosition((size + 1) * (index % 10), (size + 1) * (index / 10));
	rect.setFillColor(seed);

}

void Grass::Update()
{
	if (health >= 1.0f)
		canGrow = false;
	else if(health <= 0.0f)
		canGrow = true;

	if (canGrow){
		health += growthFactor;
		rect.setFillColor(LerpRGB(seed, mature, health));
	}
	else
	{
		health -= witherFactor;
		rect.setFillColor(LerpRGB(mature, dirt, (1.0f - health)));
	}
}

sf::RectangleShape Grass::getRect()
{
	return rect;
}

void Grass::setSize(float _size)
{
	size = _size;
}

void Grass::setIndex(int _index)
{
	index = _index;
}
