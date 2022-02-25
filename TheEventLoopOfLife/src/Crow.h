#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "Agent.h"
#include "Observer.h"
#include "Subject.h"
#include "Time.h"

class Crow :
    public Agent, public Observer, public Subject
{
public:
    Crow(sf::Vector2i _pos);
    void update(std::vector<Crow*>& _crows);
    sf::CircleShape getBody();
    sf::CircleShape getShadow();

private:
    sf::CircleShape body = sf::CircleShape(0.0f, 3);
    sf::CircleShape shadow = sf::CircleShape(0.0f, 3);
    float speed = 3.0f;
    float steerPercentage = 0.05f;
    float alignmentRadius = 2.0f;
    float cohesionRadius = 3.0f;
    float separationRadius = 0.4f;
    float shadowOffset = 0.8f;
    sf::Vector2f alignment;
    sf::Vector2f cohesion;
    sf::Vector2f separation;
    sf::Vector2f velocity;
    sf::Vector2f minVelocity;
    std::vector<Crow*> alignmentCrows;
    std::vector<Crow*> cohesionCrows;
    std::vector<Crow*> separationCrows;

    void worldWrap();
    sf::Vector2f calcAlignment();
    sf::Vector2f calcCohesion();
    sf::Vector2f calcSeparation();
    std::vector<Crow*> getCrowsInRadius(std::vector<Crow*>& _crows, float _radius);
    float magnitude(sf::Vector2f v) { return sqrt(v.x * v.x + v.y * v.y); }
    sf::Vector2f normalize(sf::Vector2f v) {
        float mag = magnitude(v);
        if (mag == 0.0f) {
            return sf::Vector2f(0.0f, 0.0f);
        }
        return sf::Vector2f(v.x / mag, v.y / mag);
    }
    sf::Vector2f invert(sf::Vector2f v) {
        return sf::Vector2f(-v.x, -v.y);
    }
};

