#pragma once

#include "Time.h"
#include "Agent.h"
#include "Subject.h"
#include "Observer.h"

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

    void sense();
    void decide();
    void act();

    sf::CircleShape getBody();

private:
    void eat();
    void breed();
    void pursue();
    void wander();
    void age();
    void die();

    float health = 0.3f;
    float currentBreedTime = 0.0f;
    const float ageFactor = 0.0f;
    const float moveTime = 0.4f;
    const float breedTime = 3.0f;
    const float breedCost = 0.7f;
    const float breedThreshold = 0.8f;
    WolfState state = WolfState::Wandering;
    sf::CircleShape body = sf::CircleShape(0.0f, 3);
};

