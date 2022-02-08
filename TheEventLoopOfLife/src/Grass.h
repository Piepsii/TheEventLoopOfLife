// Grass.h

#pragma once

#include <SFML/Graphics.hpp>
#include <random>

#include "Agent.h"
#include "Observer.h"
#include "Subject.h"

enum class GrassState {
    Seed,
    Mature,
    Dirt
};

class Grass : public Agent, public Observer, public Subject
{
public:
    Grass();
    void Create();
    void Sense();
    void Decide();
    void Act();

    void DrawDebug();
    
    sf::RectangleShape getRect();

    void setState(GrassState _state);
    void setHealth(float _health);
    int getRandomNeighborAsIndex();

    void onNotify(const Agent& _agent, Event _event) override;

    int spreadChance = 1;
    GrassState state = GrassState::Dirt;
    float health = 0.0f;
    bool marked = false;

private:
    sf::RectangleShape rect;
    sf::Color seed, mature, dirt;
    float growthFactor = 0.001f, witherFactor = 0.001f;
    bool wasTrampled = false;
    bool canGrow = true;

};


// https://www.alanzucconi.com/2016/01/06/colour-interpolation/
static sf::Color LerpRGB(sf::Color a, sf::Color b, float t)
{
    return sf::Color
    (
        a.r + (b.r - a.r) * t,
        a.g + (b.g - a.g) * t,
        a.b + (b.b - a.b) * t,
        a.a + (b.a - a.a) * t
    );
}
