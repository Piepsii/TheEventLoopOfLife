// Grass.h

#pragma once

#include "Agent.h"
#include <SFML/Graphics.hpp>

class Grass :
    public Agent
{
public:
    Grass();
    void Create();
    void Update();
    
    sf::RectangleShape getRect();

    void setSize(float _size);
    void setIndex(int _index);

private:
    sf::RectangleShape rect;
    sf::Color seed, mature, dirt;
    float size = 5.0f;
    float health = 0.0f;
    float growthFactor = 0.001f, witherFactor = 0.001f;
    unsigned int index = 0;
    bool isTrampled;
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
