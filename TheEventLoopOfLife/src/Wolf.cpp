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

void Wolf::sense(std::vector<Sheep*>& _sheepArray)
{
	sheepInSight = findSheepInACone(_sheepArray, senseRange);

	sheepInFront = findSheepInACone(_sheepArray, eatRange);

}

void Wolf::decide()
{
	if (health > breedThreshold) {
		state = WolfState::Breeding;
		return;
	}

	if (sheepInFront.size() > 0) {
		state = WolfState::Eating;
		return;
	}

	if (nearestSheep) {
		state = WolfState::Pursuing;
		return;
	}
	else {
		state = WolfState::Wandering;
		return;
	}

}

void Wolf::act()
{
	// SWITCH THE STATE MACHINE
	if (health <= 0.0f) {
		die();
	}

	switch (state) {
	case WolfState::Eating:
		body.setFillColor(sf::Color::Blue);
		eat();
		age();
		break;
	case WolfState::Breeding:
		body.setFillColor(sf::Color::Red);
		breed();
		break;
	case WolfState::Pursuing:
		body.setFillColor(sf::Color::Yellow);
		pursue();
		age();
		break;
	case WolfState::Wandering:
		body.setFillColor(sf::Color(64, 64, 64, 255));
		wander();
		age();
		break;
	}

	double pi = 2 * acos(0.0);
	float rotation = (float)(atan2(direction.y, direction.x) * 180.f / pi + 90.f);
	body.setRotation(rotation);

	auto tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);
	body.setScale(sf::Vector2f(health + 0.5f, health + 0.5f));

	for (auto sheep = sheepInSight.begin(); sheep != sheepInSight.end(); sheep++) {
		(*sheep)->markAsSeen();
	}
	if (nearestSheep) {
		sf::Vector2i distanceVec = nearestSheep->pos - pos;
		float distance = sqrtf(powf(float(distanceVec.x), 2.0f) +
							   powf(float(distanceVec.y), 2.0f));
		distanceToNearestSheep = distance;
		nearestSheep->markAsTarget();
	}
	else {
		distanceToNearestSheep = FLT_MAX;
		nearestSheep = nullptr;
	}
}

sf::CircleShape Wolf::getBody()
{
	return body;
}

void Wolf::eat()
{
	if (sheepInFront.size() > 0) {
		for (auto sheep = sheepInFront.begin(); sheep != sheepInFront.end(); sheep++) {
			(*sheep)->health -= hunger;
			health += hunger;
		}
	}
}

void Wolf::breed()
{
	currentBreedTime += Time::deltaTime;
	if (currentBreedTime > breedTime) {
		health -= breedCost;
		notify(this, Event::BREED_WOLF);
		state = WolfState::Wandering;
		currentBreedTime = 0.0f;
	}
}

void Wolf::pursue()
{
	switch (moveState) {
	case MoveState::Search:
		direction = sf::Vector2i(0, 0);
		if (nearestSheep) {
			newPos = moveTowards(nearestSheep->pos);
		}
		else {
			newPos = pos;
		}
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

std::vector<Sheep*> Wolf::findSheepInACone(std::vector<Sheep*>& _sheepArray, int _range)
{
	std::vector<Sheep*> result;
	for (auto sheep = _sheepArray.begin(); sheep != _sheepArray.end(); sheep++) {
		auto sheepPos = (*sheep)->pos;
		sf::Vector2i distanceVec = sheepPos - pos;
		float distance = sqrtf(powf(float(distanceVec.x), 2.0f) +
							   powf(float(distanceVec.y), 2.0f));
		
		if (distance < _range) {
			float dotProduct = atan2(distanceVec.y, distanceVec.x) - atan2(direction.y, direction.x);
			float pi = 2 * acos(0.0);
			dotProduct *= 180.0f / pi;
			if (abs(dotProduct) < senseAngle) {
				result.push_back(*sheep);
				if (distance < distanceToNearestSheep) {
					nearestSheep = (*sheep);
				}
			}
		}
	}
	return result;
}
