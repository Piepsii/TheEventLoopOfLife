#pragma once
#include <SFML/Graphics.hpp>

enum class MoveState {
	Search,
	Move,
	Arrive
};

class Agent
{
public:
	virtual void sense();
	virtual void decide() = 0;
	virtual void act();

	void setSize(float _size);
	void setPos(sf::Vector2i _pos);
	void setPos(int _x, int _y);

	sf::Vector2i pos;
	sf::Vector2f posf;
	sf::Vector2i direction;
	sf::Vector2i newPos;
	MoveState moveState = MoveState::Search;

protected:
	void moveTowards(sf::Vector2i _pos);
	sf::Vector2i randomAdjacentPos();
	sf::Vector2f lerpPositions(sf::Vector2i _a, sf::Vector2i _b, float _t);

	float size;
	float moveTime = 0.2f;
	float currentMoveTime = 0.0f;

};

