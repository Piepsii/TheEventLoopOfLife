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
    
    sf::RectangleShape getRect();

    void setSize(float _size);
    void setIndex(int _index);

private:
    sf::RectangleShape rect;
    float size;
    unsigned int index;
};

