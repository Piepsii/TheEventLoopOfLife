#pragma once

#include "Time.h"
#include "Agent.h"
#include "Subject.h"
#include "Observer.h"
#include "Sheep.h"

enum class MoveState;

enum class WolfState {
    Eating,
    Breeding,
    Pursuing,
    Wandering
};

class Wolf :
    public Agent, public Observer, public Subject
{
public:
    Wolf(sf::Vector2i _pos);

    void sense(std::vector<Sheep*>& _sheepArray);
    void decide();
    void act();

    sf::CircleShape getBody();

    bool debug = false;

private:
    void eat();
    void breed();
    void pursue();
    void wander();
    void age();
    void die();
    std::vector<Sheep*> findSheepInACone(std::vector<Sheep*>& _sheepArray, int _range);

    float health = 0.3f;
    float currentBreedTime = 0.0f;
    float distanceToNearestSheep = FLT_MAX;
    const float senseRange = 3.0f;
    const float eatRange = 1.0f;
    const float ageFactor = 0.006f;
    const float moveTime = 0.4f;
    const float breedTime = 3.0f;
    const float breedCost = 0.7f;
    const float hunger = 0.006f;
    const float senseAngle = 90.0f;
    const float breedThreshold = 0.8f;
    Sheep* nearestSheep;
    Sheep* sheepBeingEaten;
    std::vector<Sheep*> sheepInSight;
    std::vector<Sheep*> sheepInFront;
    WolfState state = WolfState::Wandering;
    sf::CircleShape body = sf::CircleShape(0.0f, 3);
};

