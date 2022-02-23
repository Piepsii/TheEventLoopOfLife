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
    void sense(std::vector<Grass*>& _grassArray, std::vector<sf::Vector2i*>& _wolfArray);
    void decide();
    void act();
    sf::CircleShape getBody();
    void markAsSeen();
    void markAsTarget();

    float health = 0.1f;
    bool debug = false;

private:
    void evade();
    void eat();
    void breed();
    void find();
    void wander();
    void age();
    void die();
    std::vector<Grass*> findGrassInACone(std::vector<Grass*>& _grassArray, int _range);
    sf::Vector2i calcEvadeDirection(std::vector<sf::Vector2i*>& _wolfArray, int _range);

    bool seen = false;
    bool target = false;
    float currentBreedTime = 0.0f;
    const int senseRange = 5;
    const int eatRange = 1;
    const int evadeRange = 3;
    const float moveTime = 0.6f;
    const float hunger = 0.003f;
    const float headSize = 2.0f;
    const float ageFactor = 0.001f;
    const float breedThreshold = 0.9f;
    const float breedCost = 0.5f;
    const float breedTime = 1.5f;
    Grass* grassBelow;
    Grass* nearestMatureGrass;
    Grass* grassBeingGrazed;
    std::vector<Grass*> grassInSight;
    std::vector<Grass*> grassInFront;
    std::vector<sf::Vector2i*> sensedWolves;
    SheepState state = SheepState::Wandering;
    sf::CircleShape body = sf::CircleShape();

    float magnitude(sf::Vector2i v) { return sqrt(v.x * v.x + v.y * v.y); }
};
