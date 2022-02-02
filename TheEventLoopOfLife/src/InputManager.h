#pragma once
#include "SFML/Window/Event.hpp"
class InputManager
{
public:
	InputManager();

	void setKey(sf::Keyboard::Key _key, bool _pressed);
	void Update();
	bool isKeyDown(sf::Keyboard::Key _key);
	bool isKeyPressed(sf::Keyboard::Key _key);
	bool isKeyReleased(sf::Keyboard::Key _key);

private:
	bool keys[sf::Keyboard::KeyCount];
	bool previousKeys[sf::Keyboard::KeyCount];
};

