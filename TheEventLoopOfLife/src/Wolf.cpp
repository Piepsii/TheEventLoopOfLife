#include "Wolf.h"

Wolf::Wolf(sf::Vector2i _pos)
{
	pos = _pos;
	size = Grid::Instance()->TileSize() * 0.2f;
	body.setRadius(size);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setFillColor(sf::Color(128));
}

void Wolf::sense()
{
}

void Wolf::decide()
{
}

void Wolf::act()
{
}

sf::CircleShape Wolf::getBody()
{
	return body;
}

void Wolf::wander()
{
	switch (moveState) {
	case MoveState::Search:
		direction = sf::Vector2i(0, 0);
		newPos = randomAdjacentPos();
		moveState = MoveState::Move;
		break;

	case MoveState::Move:
		currentMoveTime += Time::deltaTime;
		posf = lerpPositions(pos, newPos, currentMoveTime / moveTime);
		if (currentMoveTime >= moveTime)
			moveState = MoveState::Arrive;
		break;

	case MoveState::Arrive:
		currentMoveTime = 0.0f;
		pos = newPos;
		notify(this, Event::TRAMPLE);
		moveState = MoveState::Search;
		break;
	}
}
