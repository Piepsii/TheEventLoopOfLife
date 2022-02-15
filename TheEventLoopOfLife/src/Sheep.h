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

class Sheep :
    public Agent, public Observer, public Subject
{
public:
    Sheep(sf::Vector2i _pos);

    void sense(std::vector<Grass*>& _grassArray);
    void decide();
    void act(std::vector<Grass*>& _grassArray);

    std::vector<Grass*> findGrassInACone(std::vector<Grass*>& _grassArray, int _range);

    void updateShape(float _tileSize);
    sf::CircleShape getBody();

    bool debug = false;
private:
    void eat();
    void breed();
    void find();
    void wander();
    void age();
    void die();

    float health = 0.3f;
    const int senseRange = 5;
    const int eatRange = 1;
    const float hunger = 0.001f;
    const float headSize = 2.0f;
    const float ageFactor = 0.001f;
    const float breedThreshold = 0.9f;
    const float breedCost = 0.5f;
    Grass* grassBelow;
    Grass* nearestMatureGrass;
    Grass* grassBeingGrazed;
    SheepState state = SheepState::Wandering;
    sf::CircleShape body = sf::CircleShape();
    std::vector<Grass*> grassInSight;
    std::vector<Grass*> grassInFront;
};
