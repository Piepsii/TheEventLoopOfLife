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
    Sheep();
    void create(float _tileSize);

    void sense(std::vector<Grass*>& _grassArray);
    void decide();
    void act(std::vector<Grass*>& _grassArray);

    std::vector<Grass*> findGrassInACone(std::vector<Grass*>& _grassArray, int _range);

    void updateShape(float _tileSize);
    sf::CircleShape getBody();
    sf::CircleShape getHead();

    bool debug = false;
private:
    void eat();
    void breed();
    void find();
    void wander();
    void age();
    void die();
    void moveTowards(sf::Vector2i _pos);

    int senseRange = 5;
    int eatRange = 1;
    float health = 1.0f;
    float hunger = 0.01f;
    float headSize = 2.0f;
    Grass* grassBelow;
    Grass* nearestMatureGrass;
    SheepState state = SheepState::Wandering;
    sf::CircleShape body;
    sf::CircleShape head;
    std::vector<Grass*> grassInSight;
    std::vector<Grass*> grassInFront;
};
