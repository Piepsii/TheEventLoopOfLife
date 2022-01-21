#include "Grass.h"

Grass::Grass()
{
}

void Grass::Create()
{
	rect = sf::RectangleShape(sf::Vector2f(size, size));
	rect.setPosition((size + 1) * (index % 10), (size + 1) * (index / 10));
	rect.setFillColor(sf::Color::Yellow);
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
