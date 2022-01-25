#pragma once
#include "Agent.h"
#include <SFML/Graphics.hpp>

class Sheep :
    public Agent
{
public:
    Sheep();
    void Create(float _tileSize);

    void Sense();
    void Decide();
    void Act();
    sf::CircleShape getCircle();

private:
    void Eat();
    void Breed();
    void Find();
    void Wander();
    void Age();
    void Die();

    float health = 1.0f;
    sf::CircleShape circle;
};

