#pragma once
#include "Agent.h"
#include <SFML/Graphics.hpp>
#include "Grass.h"

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

    void Sense(Grass& _grassBelow, Grass* _grassInFront);
    void Decide();
    void Act();

    void updateShape(float _tileSize);
    sf::CircleShape getBody();
    sf::CircleShape getHead();

private:
    void Eat();
    void Breed();
    void Find();
    void Wander();
    void Age();
    void Die();

    float health = 1.0f;
    float moveSpeed = 0.001f;
    float hunger = 0.01f;
    float headSize = 2.0f;
    sf::CircleShape body, head;
    SheepState state = SheepState::Wandering;
    Grass* grassBelow = nullptr;
    Grass* grassInFront = nullptr;
    sf::Vector2i direction = sf::Vector2i(0, 0);
};

static float lerp(float a, float b, float f)
{
    return (a * (1.0 - f)) + (b * f);
}

