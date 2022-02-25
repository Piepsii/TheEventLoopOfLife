// Grass.h

#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include <algorithm>

#include "Agent.h"
#include "Observer.h"
#include "Subject.h"

enum class GrassState {
    SEED,
    MATURE,
    DIRT
};

enum class GridState {
    CORNER,
    EDGE,
    CENTRAL
};

enum class DebugColor {
    DEFAULT,
    RED,
    BLUE,
    YELLOW
};

class Grass : public Agent, public Observer, public Subject
{
public:
    Grass();
    void create();
    void sense();
    void decide();
    void act();
    void drawDebug();
    sf::RectangleShape getRect();
    void setState(GrassState _state);
    void setHealth(float _health);
    int getRandomNeighborAsIndex();
    void onNotify(const Agent& _agent, Event _event) override;

    int spreadChance = 1;
    GrassState state = GrassState::DIRT;
    GridState gridState = GridState::CENTRAL;
    float health = 0.0f;
    DebugColor debugColor = DebugColor::DEFAULT;

private:
    sf::RectangleShape rect;
    sf::Color seed, mature, dirt;
    float growthFactor = 0.01f, witherFactor = 0.001f;
    bool wasTrampled = false;
    bool canGrow = true;

};

// https://www.alanzucconi.com/2016/01/06/colour-interpolation/
static sf::Color LerpRGB(sf::Color a, sf::Color b, float t)
{
    const uint8_t UINT8_MIN = 0;
    return sf::Color
    (
        std::clamp(sf::Uint8(a.r + (b.r - a.r) * t), UINT8_MIN, UINT8_MAX),
        std::clamp(sf::Uint8(a.g + (b.g - a.g) * t), UINT8_MIN, UINT8_MAX),
        std::clamp(sf::Uint8(a.b + (b.b - a.b) * t), UINT8_MIN, UINT8_MAX),
        std::clamp(sf::Uint8(a.a + (b.a - a.a) * t), UINT8_MIN, UINT8_MAX)
    );
}
