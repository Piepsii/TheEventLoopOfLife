#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Agent.h"
#include "Grass.h"
#include "Subject.h"
#include "Observer.h"

enum class SheepState {
    Evading,
    Eating,
    Breeding,
    Finding,
    Wandering
};

enum class MoveState {
    Search,
    Move,
    Arrive
};

class Sheep :
    public Agent, public Observer, public Subject
{
public:
    Sheep();
    void Create(float _tileSize);

    void Sense();
    void Decide();
    void Act(std::vector<Grass*>& _grassArray);

    void CalculateLineOfSight(std::vector<Grass*>& _grassArray);

    void updateShape(float _tileSize);
    sf::CircleShape getBody();
    sf::CircleShape getHead();

    bool debug = false;
private:
    void Eat();
    void Breed();
    void Find();
    void Wander();
    void Age();
    void Die();

    int senseRange = 5;
    int eatRange = 1;
    float health = 1.0f;
    float moveTime = 0.2f, currentMoveTime = 0.0f;
    float hunger = 0.01f;
    float headSize = 2.0f;
    sf::CircleShape body, head;
    SheepState state = SheepState::Wandering;
    MoveState moveState = MoveState::Search;
    Grass* grassBelow = nullptr;
    std::vector<Grass*> grassInFront = {};
    sf::Vector2i direction;
    sf::Vector2i newPos;
};
