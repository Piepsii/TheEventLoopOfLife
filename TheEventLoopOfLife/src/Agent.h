#pragma once

#include <SFML/Graphics.hpp>

#include "Grid.h"

enum class MoveState {
	Search,
	Move,
	Arrive
};

class Agent
{
public:
	Agent();
	virtual void sense();
	virtual void decide();
	virtual void act();
	bool isSensingDeciding();
	void setSize(float _size);
	void setPos(sf::Vector2i _pos);
	void setPos(int _x, int _y);

	float senseDecideCounter;
	float senseDecideFrequency;
	static uint32_t idCounter;
	uint32_t id;
	sf::Vector2i pos;
	sf::Vector2f posf;
	sf::Vector2i direction;
	sf::Vector2i newPos;
	MoveState moveState;

protected:
	sf::Vector2i moveTowards(sf::Vector2i _pos);
	sf::Vector2i randomAdjacentPos();
	sf::Vector2f lerpPositions(sf::Vector2f _a, sf::Vector2f _b, float _t);

	float size = float(Grid::Instance()->TileSize());
	float moveTime;
	float currentMoveTime;

};
