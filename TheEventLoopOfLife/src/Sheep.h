#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Agent.h"
#include "Grass.h"
#include "Subject.h"
#include "Observer.h"

enum class SheepState {
    EVADING,
    EATING,
    BREEDING,
    FINDING,
    WANDERING
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

    float health;
    bool debug;

private:
    void evade();
    void eat();
    void breed();
    void find();
    void wander();
    void age();
    void die();
    std::vector<Grass*> findGrassInACone(std::vector<Grass*>& _grassArray, int _range);
    sf::Vector2i calcEvadeDirection(std::vector<sf::Vector2i*>& _wolfArray);

    bool seen;
    bool target;
    bool isBeingEaten;
    float currentBreedTime;
    const int senseRange;
    const int eatRange;
    const int evadeRange;
    const float moveTime;
    const float hunger;
    const float headSize;
    const float ageFactor;
    const float breedThreshold;
    const float breedCost;
    const float breedTime;
    Grass* grassBelow{};
    Grass* nearestMatureGrass{};
    Grass* grassBeingGrazed{};
    std::vector<Grass*> grassInSight;
    std::vector<Grass*> grassInFront;
    std::vector<sf::Vector2i*> sensedWolves;
    SheepState state;
    sf::CircleShape body;

    float magnitude(sf::Vector2i v) { return sqrtf(v.x * v.x + v.y * v.y); }
};
