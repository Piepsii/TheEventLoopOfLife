#include "Wolf.h"

Wolf::Wolf(sf::Vector2i _pos)
{
	pos = _pos;
	size = Grid::Instance()->TileSize() * 0.5f;
	body.setRadius(size);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setFillColor(sf::Color(64, 64, 64, 255));
}

void Wolf::sense()
{
}

void Wolf::decide()
{
}

void Wolf::act()
{
	// SWITCH THE STATE MACHINE
	age();
	if (health <= 0.0f) {
		die();
	}

	switch (state) {
	case WolfState::Eating:
		eat();
		break;
	case WolfState::Breeding:
		breed();
		break;
	case WolfState::Pursuing:
		pursue();
		break;
	case WolfState::Wandering:
		wander();
		break;
	}

	double pi = 2 * acos(0.0);
	float rotation = (float)(atan2(direction.y, direction.x) * 180.f / pi + 90.f);
	body.setRotation(rotation);

	auto tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);
}

sf::CircleShape Wolf::getBody()
{
	return body;
}

void Wolf::eat()
{
}

void Wolf::breed()
{
}

void Wolf::pursue()
{
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
		posf = lerpPositions(sf::Vector2f(pos.x, pos.y), sf::Vector2f(newPos.x, newPos.y), currentMoveTime / moveTime);
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

void Wolf::age()
{
	health -= ageFactor * Time::deltaTime;
}

void Wolf::die()
{
	notify(this, Event::DEATH);
}
