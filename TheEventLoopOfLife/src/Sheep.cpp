#include "Sheep.h"

Sheep::Sheep()
{
	state = SheepState::Wandering;
}

void Sheep::Create(float _tileSize)
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
}

void Sheep::Sense()
{

	// GATHER DATA
	// is there a wolf nearby?
	// can i reproduce?
	// am i hungry?
}

void Sheep::Decide()
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
}

void Sheep::Act(std::vector<Grass>& _grassArray)
{
	// SWITCH THE STATE MACHINE
	Age();
	if (health <= 0.0f) {
		Die();
	}

	switch (state) {
	case SheepState::Evading:
		
		break;
	case SheepState::Eating:
		Eat();
		break;
	case SheepState::Breeding:
		Breed();
		break;
	case SheepState::Finding:
		Find();
		break;
	case SheepState::Wandering:
		Wander();
		break;
	}

	CalculateLineOfSight(_grassArray);
	
	if (debug) {
		for (int i = 0; i < grassInFront.size(); i++) {
			grassInFront[i]->marked = true;
		}
	}
}

void Sheep::CalculateLineOfSight(std::vector<Grass>& _grassArray)
{
	int grassAmount = _grassArray.size();
	int gridSize = sqrt(grassAmount);
	int index = pos.x + pos.y * gridSize;
	grassBelow = &_grassArray[index];
	grassInFront.clear();

	for (int yOffset = 0, x = 1; x <= 1 + senseRange * 2; x += 2, yOffset--) {
		for (int y = yOffset; y <= -yOffset; y++) {
			sf::Vector2i grasPos = direction.y == 0
				? sf::Vector2i(pos.x - yOffset * direction.x, pos.y + y)
				: sf::Vector2i(pos.x + y, pos.y - yOffset * direction.y);
			if (grasPos.x >= gridSize || grasPos.x < 0 || grasPos.y >= gridSize|| grasPos.y < 0)
				continue;
			int gridIndex = grasPos.x + grasPos.y * gridSize;
			grassInFront.push_back(&_grassArray[gridIndex]);
		}
	}
}

void Sheep::updateShape(float _tileSize)
{
	body.setPosition((_tileSize + 1) * pos.x + _tileSize / 2,
					   (_tileSize + 1) * pos.y + _tileSize / 2);
	head.setPosition((_tileSize + 1) * pos.x + _tileSize / 2 + direction.x * size,
					 (_tileSize + 1) * pos.y + _tileSize / 2 + direction.y * size);
}

sf::CircleShape Sheep::getBody()
{
	return body;
}

sf::CircleShape Sheep::getHead()
{
	return head;
}

void Sheep::Eat()
{
	body.setOutlineThickness(3.0f);
	grassBelow->health -= hunger;
	health += hunger;
}

void Sheep::Breed()
{
}

void Sheep::Find()
{
}

void Sheep::Wander()
{
	body.setOutlineThickness(1.0f);
	sf::Vector2f vec = body.getPosition();

	sf::Vector2i newPos = pos;
	direction = sf::Vector2i(0, 0);
	int moveX;
	do {
		moveX = rand() % 3 - 1;
		newPos.x += moveX;
		direction.x = moveX;
	} while (newPos.x < 0 || newPos.x > 9);

	if (moveX == 0) {
		do {
			int moveY = rand() % 2 * 2 - 1;
			newPos.y += moveY;
			direction.y = moveY;
		} while (newPos.y < 0 || newPos.y > 9);
	}
	pos = newPos;
}

void Sheep::Age()
{
	health -= 0.0001f;
}

void Sheep::Die()
{
	body.setFillColor(sf::Color::Red);
}
