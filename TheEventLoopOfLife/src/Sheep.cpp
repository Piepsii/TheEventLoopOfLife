#include "Sheep.h"
#include "Time.h"

Sheep::Sheep(sf::Vector2i _pos) :
	health(0.1f),
	debug(false),
	seen(false),
	target(false),
	isBeingEaten(false),
	currentBreedTime(0.0f),
	senseRange(5),
	eatRange(1),
	evadeRange(3),
	moveTime(0.5f),
	hunger(0.003f),
	headSize(2.0f),
	ageFactor(0.001f),
	breedThreshold(0.9f),
	breedCost(0.5f),
	breedTime(1.5f),
	grassBelow(nullptr),
	nearestMatureGrass(nullptr),
	grassBeingGrazed(nullptr),
	state(SheepState::WANDERING)

{
	pos = _pos;
	size = Grid::Instance()->TileSize() * 0.2f;
	body.setRadius(size);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setFillColor(sf::Color::White);

	float tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);
	body.setScale(sf::Vector2f(health + 0.5f, health + 0.5f));
	senseDecideFrequency = 0.2f;
}

void Sheep::sense(std::vector<Grass*>& _grassArray, std::vector<sf::Vector2i*>& _wolfArray)
{
	grassInSight = findGrassInACone(_grassArray, senseRange);

	for (auto grass = grassInSight.begin(); grass != grassInSight.end(); grass++) {
		if ((*grass)->state == GrassState::MATURE) {
			nearestMatureGrass = (*grass);
			break;
		}
	}

	grassInFront = findGrassInACone(_grassArray, eatRange);

	for (auto grass = grassInFront.begin(); grass != grassInFront.end(); grass++) {
		if ((*grass)->state == GrassState::MATURE) {
			grassBeingGrazed = (*grass);
		}
	}

	sensedWolves.clear();
	for (auto wolf = _wolfArray.begin(); wolf != _wolfArray.end(); wolf++) {
		sf::Vector2i distance = (**wolf) - pos;
		if (magnitude(distance) < evadeRange) {
			sensedWolves.push_back(*wolf);
		}
	}
}

void Sheep::decide()
{
	if (sensedWolves.size() > 0) {
		state = SheepState::EVADING;
		return;
	}

	if (health > breedThreshold) {
		state = SheepState::BREEDING;
		return;
	}

	if (nearestMatureGrass) {
		if (nearestMatureGrass->state != GrassState::MATURE) {
			nearestMatureGrass = nullptr;
			state = SheepState::WANDERING;
		}
		state = SheepState::FINDING;
	}
	else {
		state = SheepState::WANDERING;
	}


	if (grassBeingGrazed) {
		if (grassBeingGrazed->state == GrassState::MATURE) {
			state = SheepState::EATING;
		}
		else {
			state = SheepState::WANDERING;
			grassBeingGrazed = nullptr;
		}
	}

}

void Sheep::act()
{
	// SWITCH THE STATE MACHINE

	switch (state) {
	case SheepState::EVADING:
		body.setFillColor(sf::Color::Green);
		evade();
		age();
		break;
	case SheepState::EATING:
		body.setFillColor(sf::Color::Blue);
		eat();
		age();
		break;
	case SheepState::BREEDING:
		body.setFillColor(sf::Color::Red);
		breed();
		break;
	case SheepState::FINDING:
		body.setFillColor(sf::Color::Yellow);
		find();
		age();
		break;
	case SheepState::WANDERING:
		body.setFillColor(sf::Color::White);
		wander();
		age();
		break;
	}

	float tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);
	body.setScale(sf::Vector2f(health + 0.5f, health + 0.5f));

	body.setOutlineThickness(1.0f);
	if (seen) {
		body.setOutlineThickness(5.0f);
		seen = false;
	}

	body.setOutlineColor(sf::Color::Black);
	if (target) {
		body.setOutlineColor(sf::Color::Cyan);
		target = false;
	}

	if (debug) {
		for (int i = 0; i < grassInSight.size(); i++) {
			grassInSight[i]->debugColor = DebugColor::RED;
		}
		for (int i = 0; i < grassInFront.size(); i++) {
			grassInFront[i]->debugColor = DebugColor::BLUE;
		}
		if(nearestMatureGrass)
			nearestMatureGrass->debugColor = DebugColor::YELLOW;
		if(grassBeingGrazed)
			grassBeingGrazed->debugColor = DebugColor::YELLOW;
	}

	if (health <= 0.0f) {
		die();
	}

}

std::vector<Grass*> Sheep::findGrassInACone(std::vector<Grass*>& _grassArray, int _range)
{
	std::vector<Grass*> result;
	int grassAmount = (int)_grassArray.size();
	int gridSize = Grid::Instance()->Columns();
	int index = pos.x + pos.y * gridSize;
	if (index < 0 || index > grassAmount)
		return result;

	grassBelow = _grassArray[index];

	for (int yOffset = 0, x = 1; x <= 1 + _range * 2; x += 2, yOffset--) {
		for (int y = yOffset; y <= -yOffset; y++) {
			sf::Vector2i grasPos = direction.y == 0
				? sf::Vector2i(pos.x - yOffset * direction.x, pos.y + y)
				: sf::Vector2i(pos.x + y, pos.y - yOffset * direction.y);
			if (grasPos.x >= gridSize || grasPos.x < 0 || grasPos.y >= gridSize|| grasPos.y < 0)
				continue;
			int gridIndex = grasPos.x + grasPos.y * gridSize;
			result.push_back(_grassArray[gridIndex]);
		}
	}
	return result;
}

sf::Vector2i Sheep::calcEvadeDirection(std::vector<sf::Vector2i*>& _wolfArray)
{
	// index = direction of danger
	// 0 = North, 1 = East, 2 = South, 3 = West
	int threatLevel[4] = { 0, 0, 0, 0 };
	for (auto wolf = _wolfArray.begin(); wolf != _wolfArray.end(); wolf++) {
		sf::Vector2i vec = (**wolf) - pos;
		if (vec.y < 0.f && vec.x < vec.y && vec.x > -vec.y) {
			threatLevel[0]++;
		}
		else if (vec.x > 0 && vec.y < vec.x && vec.y > -vec.x) {
			threatLevel[1]++;
		}
		else if (vec.y > 0 && vec.x < vec.y && vec.x > -vec.y) {
			threatLevel[2]++;
		}
		else if (vec.x < 0 && vec.y < vec.x && vec.y > -vec.x) {
			threatLevel[3]++;
		}
		else if (vec.x == 0 && vec.y == 0) {
			isBeingEaten = true;
			return sf::Vector2i{ 0, 0 };
		}
	}
	int index = -1, highest = 0;
	for (int i = 0; i < 4; i++) {
		if (threatLevel[i] > highest) {
			highest = threatLevel[i];
			index = i;
		}
	}

	if (pos.y == 0) {
		return sf::Vector2i{ 0, 1 };
	}
	else if (pos.x == (int)Grid::Instance()->Columns()) {
		return sf::Vector2i{ -1, 0 };
	}
	else if (pos.y == (int)Grid::Instance()->Rows()) {
		return sf::Vector2i{ 0, -1 };
	}
	else if(pos.x == 0){
		return sf::Vector2i{ 1, 0 };
	}

	if (index == 0) {
		return sf::Vector2i{ 0, 1 };
	} else if (index == 1) {
		return sf::Vector2i{ -1, 0 };
	} else if (index == 2) {
		return sf::Vector2i{ 0, -1 };
	} else if (index == 3) {
		return sf::Vector2i{ 1, 0 };
	}
	return sf::Vector2i{ 0, 0 };
}

sf::CircleShape Sheep::getBody()
{
	return body;
}

void Sheep::markAsSeen()
{
	seen = true;
}

void Sheep::markAsTarget()
{
	target = true;
}

void Sheep::evade() {
	switch (moveState) {
	case MoveState::Search:
		direction = sf::Vector2i(0, 0);
		newPos += calcEvadeDirection(sensedWolves);
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

void Sheep::eat()
{
	if(debug)
		grassBeingGrazed->debugColor = DebugColor::BLUE;
	grassBeingGrazed->health -= hunger;
	health += hunger;
}

void Sheep::breed()
{
	currentBreedTime += Time::deltaTime;
	if (currentBreedTime > breedTime) {
		health -= breedCost;
		notify(this, Event::BREED_SHEEP);
		state = SheepState::WANDERING;
		currentBreedTime = 0.0f;
	}
}

void Sheep::find()
{
	if(nearestMatureGrass && debug)
		nearestMatureGrass->debugColor = DebugColor::YELLOW;
	switch (moveState) {
	case MoveState::Search:
		direction = sf::Vector2i(0, 0);
		if (nearestMatureGrass) {
			newPos = moveTowards(nearestMatureGrass->pos);
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

void Sheep::wander()
{
	switch(moveState){
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

void Sheep::age()
{
	health -= ageFactor;
}

void Sheep::die()
{
	notify(this, Event::DEATH_SHEEP);
}
