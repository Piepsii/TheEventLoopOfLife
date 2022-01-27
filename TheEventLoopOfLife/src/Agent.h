#pragma once
#include <SFML/Graphics.hpp>

class Agent
{
public:
	virtual void Sense();
	virtual void Decide() = 0;
	virtual void Act() = 0;

	void setSize(float _size);
	void setPos(sf::Vector2i _pos);
	void setPos(int _x, int _y);

	sf::Vector2i pos = sf::Vector2i(0, 0);

protected:
	float size;

};

