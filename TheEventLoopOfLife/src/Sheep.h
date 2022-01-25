#pragma once
#include "Agent.h"
#include <SFML/Graphics.hpp>

enum class SheepState {
    Evading,
    Eating,
    Breeding,
    Finding,
    Wandering
};

class Sheep :
    public Agent
{
public:
    Sheep();
    void Create(float _tileSize);

    void Sense();
    void Decide();
    void Act();

    void updateShape(float _tileSize);
    sf::CircleShape getCircle();

private:
    void Eat();
    void Breed();
    void Find();
    void Wander();
    void Age();
    void Die();

    float health = 1.0f;
    float moveSpeed = 0.001f;
    sf::CircleShape circle;
    SheepState state = SheepState::Wandering;
};

static float lerp(float a, float b, float f)
{
    return (a * (1.0 - f)) + (b * f);
}

