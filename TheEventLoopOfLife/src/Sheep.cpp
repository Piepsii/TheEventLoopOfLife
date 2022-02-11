#include "Sheep.h"
#include "Time.h"

Sheep::Sheep()
{
	state = SheepState::Wandering;
}

void Sheep::create(float _tileSize)
{
	body = sf::CircleShape();
	body.setRadius(size);
	body.setOutlineColor(sf::Color::Black);
	body.setOrigin(size, size);
	body.setPosition((_tileSize + 1) * pos.x + _tileSize / 2,
					   (_tileSize + 1) * pos.y + _tileSize / 2);
	body.setFillColor(sf::Color::White);

	head = sf::CircleShape();
	head.setRadius(size / headSize);
	head.setOrigin(size / headSize, size / headSize);
	head.setPosition((_tileSize + 1) * pos.x + _tileSize / 2,
					 (_tileSize + 1) * pos.y + _tileSize / 2);
	head.setFillColor(sf::Color::Black);
	body.setOutlineThickness(1.0f);
}

void Sheep::sense(std::vector<Grass*>& _grassArray)
{
	// GATHER DATA
	// is there a wolf nearby?
	// can i reproduce?
	// am i hungry?

	grassInSight = findGrassInACone(_grassArray, senseRange);
	grassInFront = findGrassInACone(_grassArray, eatRange);

	for (auto grass = grassInSight.begin(); grass != grassInSight.end(); grass++) {
		if ((*grass)->state == GrassState::Mature) {
			nearestMatureGrass = (*grass);
			break;
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
	if (nearestMatureGrass) {
		state = SheepState::Finding;
	}
}

void Sheep::act(std::vector<Grass*>& _grassArray)
{
	// SWITCH THE STATE MACHINE
	age();
	if (health <= 0.0f) {
		die();
	}

	switch (state) {
	case SheepState::Evading:
		
		break;
	case SheepState::Eating:
		eat();
		break;
	case SheepState::Breeding:
		breed();
		break;
	case SheepState::Finding:
		find();
		break;
	case SheepState::Wandering:
		wander();
		break;
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
	}
}

std::vector<Grass*> Sheep::findGrassInACone(std::vector<Grass*>& _grassArray, int _range)
{
	std::vector<Grass*> result;
	int grassAmount = _grassArray.size();
	int gridSize = sqrt(grassAmount);
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

void Sheep::updateShape(float _tileSize)
{
	body.setPosition((_tileSize + 1) * posf.x + _tileSize / 2,
					   (_tileSize + 1) * posf.y + _tileSize / 2);
	head.setPosition((_tileSize + 1) * posf.x + _tileSize / 2 + direction.x * size,
					 (_tileSize + 1) * posf.y + _tileSize / 2 + direction.y * size);
}

sf::CircleShape Sheep::getBody()
{
	return body;
}

sf::CircleShape Sheep::getHead()
{
	return head;
}

void Sheep::eat()
{
	body.setOutlineThickness(3.0f);
	grassBelow->health -= hunger;
	health += hunger;
}

void Sheep::breed()
{
}

void Sheep::find()
{
}

void Sheep::wander()
{
	switch(moveState){
	case MoveState::Search:
		newPos = pos;
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
		notify(this, Event::EVENT_TRAMPLE);
		moveState = MoveState::Search;
		break;
	}
}

void Sheep::age()
{
	health -= 0.0001f;
}

void Sheep::die()
{
	body.setFillColor(sf::Color::Red);
}
