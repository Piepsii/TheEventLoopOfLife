#include "Agent.h"

void Agent::sense()
{
}

void Agent::decide() 
{
}

void Agent::act()
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

sf::Vector2i Agent::moveTowards(sf::Vector2i _pos) {
	sf::Vector2i result = pos;
	sf::Vector2i path = _pos - pos;
	int absX = std::abs(path.x);
	int absY = std::abs(path.y);
	if (absX > std::abs(path.y)) {
		direction.x = absX != 0 ? path.x / absX : 0; 
		result.x += direction.x;
	}
	else {
		direction.y = absY != 0 ? path.y / absY : 0;
		result.y += direction.y;
	}
	return result;
}

sf::Vector2i Agent::randomAdjacentPos()
{
	sf::Vector2i result = pos;
	int moveX;
	do {
		moveX = rand() % 3 - 1;
		result.x += moveX;
		direction.x = moveX;
	} while (result.x < 0 || result.x >= Grid::Instance()->Columns());
	if (moveX == 0) {
		do {
			int moveY = rand() % 2 * 2 - 1;
			result.y += moveY;
			direction.y = moveY;
		} while (result.y < 0 || result.y >= Grid::Instance()->Rows());
	}
	return result;
}

sf::Vector2f Agent::lerpPositions(sf::Vector2f _a, sf::Vector2f _b, float _t)
{
	sf::Vector2f result;
	result.x = std::lerp(_a.x, _b.x, _t);
	result.y = std::lerp(_a.y, _b.y, _t);
	return result;
}
