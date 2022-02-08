#pragma once

#include <SFML/Graphics.hpp>

#include "Agent.h"
#include "Grass.h"
#include "Subject.h"

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
    void Act(std::vector<Grass>& _grassArray);

    void CalculateLineOfSight(std::vector<Grass>& _grassArray);

    Subject Ate();

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
    float moveSpeed = 0.001f;
    float hunger = 0.01f;
    float headSize = 2.0f;
    sf::CircleShape body, head;
    SheepState state = SheepState::Wandering;
    Grass* grassBelow = nullptr;
    std::vector<Grass*> grassInFront = {};
    sf::Vector2i direction = sf::Vector2i(0, 0);

    Subject ate;
};

static float lerp(float a, float b, float f)
{
    return (a * (1.0 - f)) + (b * f);
}

