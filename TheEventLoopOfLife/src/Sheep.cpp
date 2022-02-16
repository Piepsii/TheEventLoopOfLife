#include "Sheep.h"
#include "Time.h"

Sheep::Sheep(sf::Vector2i _pos)
{
	pos = _pos;
	size = Grid::Instance()->TileSize() * 0.2f;
	body.setRadius(size);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setFillColor(sf::Color::White);
}

void Sheep::sense(std::vector<Grass*>& _grassArray)
{
	// GATHER DATA
	// is there a wolf nearby?
	// can i reproduce?
	// am i hungry?

	grassInSight = findGrassInACone(_grassArray, senseRange);

	for (auto grass = grassInSight.begin(); grass != grassInSight.end(); grass++) {
		if ((*grass)->state == GrassState::Mature) {
			nearestMatureGrass = (*grass);
			break;
		}
	}

	grassInFront = findGrassInACone(_grassArray, eatRange);

	for (auto grass = grassInFront.begin(); grass != grassInFront.end(); grass++) {
		if ((*grass)->state == GrassState::Mature) {
			grassBeingGrazed = (*grass);
		}
	}
}

void Sheep::decide()
{
	// SET THE STATE MACHINE
	// if standing on grass & hungry > Eating
	//if (grassBelow->state == GrassState::Mature) {
	//	state = SheepState::Eating;
	//	return;
	//}
	//// if able to reproduce > breeding
	//// if hungry & near grass > finding
	//// if not near grass > wandering
	//if (grassBelow->state != GrassState::Mature) {
	//	state = SheepState::Wandering;
	//	return;
	//}
	if (health > breedThreshold) {
		state = SheepState::Breeding;
		return;
	}

	if (nearestMatureGrass) {
		if (nearestMatureGrass->state != GrassState::Mature) {
			nearestMatureGrass = nullptr;
			state = SheepState::Wandering;
		}
		state = SheepState::Finding;
	}
	else {
		state = SheepState::Wandering;
	}


	if (grassBeingGrazed) {
		if (grassBeingGrazed->state == GrassState::Mature) {
			state = SheepState::Eating;
		}
		else {
			state = SheepState::Wandering;
			grassBeingGrazed = nullptr;
		}
	}

}

void Sheep::act(std::vector<Grass*>& _grassArray)
{
	// SWITCH THE STATE MACHINE

	if (health <= 0.0f) {
		die();
	}

	switch (state) {
	case SheepState::Evading:
		
		break;
	case SheepState::Eating:
		body.setFillColor(sf::Color::Blue);
		eat();
		age();
		break;
	case SheepState::Breeding:
		body.setFillColor(sf::Color::Red);
		breed();
		break;
	case SheepState::Finding:
		body.setFillColor(sf::Color::Yellow);
		find();
		age();
		break;
	case SheepState::Wandering:
		body.setFillColor(sf::Color::White);
		wander();
		age();
		break;
	}

	float tileSize = Grid::Instance()->TileSize();
	body.setPosition((tileSize + 1) * posf.x + tileSize / 2,
					 (tileSize + 1) * posf.y + tileSize / 2);
	body.setScale(sf::Vector2f(health + 0.5f, health + 0.5f));

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
}

std::vector<Grass*> Sheep::findGrassInACone(std::vector<Grass*>& _grassArray, int _range)
{
	std::vector<Grass*> result;
	int grassAmount = _grassArray.size();
	int gridSize = Grid::Instance()->Columns();
	int index = pos.x + pos.y * gridSize;
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

sf::CircleShape Sheep::getBody()
{
	return body;
}

void Sheep::eat()
{
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
		state = SheepState::Wandering;
		currentBreedTime = 0.0f;
	}
}

void Sheep::find()
{
	if(nearestMatureGrass)
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
	notify(this, Event::DEATH);
}
