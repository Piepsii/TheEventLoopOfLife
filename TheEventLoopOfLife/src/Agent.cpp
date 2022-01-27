#include "Agent.h"

void Agent::Sense()
{
}

void Agent::setSize(float _size)
{
	size = _size;
}

void Agent::setPos(sf::Vector2i _pos)
{
	pos = _pos;
}

void Agent::setPos(int _x, int _y)
{
	pos.x = _x;
	pos.y = _y;
}
