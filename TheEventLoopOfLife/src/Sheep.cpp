#include "Sheep.h"

Sheep::Sheep()
{
	state = SheepState::Wandering;
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

	switch (state) {
	case SheepState::Evading:
		
		break;
	case SheepState::Eating:
		Eat();
		break;
	case SheepState::Breeding:
		Breed();
		break;
	case SheepState::Finding:
		Find();
		break;
	case SheepState::Wandering:
		Wander();
		break;
	}
}

void Sheep::updateShape(float _tileSize)
{
	circle.setPosition((_tileSize + 1) * pos.x + _tileSize / 2,
					   (_tileSize + 1) * pos.y + _tileSize / 2);
}

sf::CircleShape Sheep::getCircle()
{
	return circle;
}

void Sheep::Eat()
{

}

void Sheep::Breed()
{
}

void Sheep::Find()
{
}

void Sheep::Wander()
{
	sf::Vector2f vec = circle.getPosition();
	int moveX = rand() % 3 - 1;
	pos.x += moveX;
	int moveY = rand() % 3 - 1;
	pos.y += moveY;

}

void Sheep::Age()
{
	health -= 0.0001f;
}

void Sheep::Die()
{
	circle.setFillColor(sf::Color::Red);
}
