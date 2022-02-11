#include "InputManager.h"

InputManager::InputManager()
{
	for (int key = 0; key < sf::Keyboard::KeyCount; key++)
	{
		keys[key] = false;
		previousKeys[key] = false;
	}
}

void InputManager::setKey(sf::Keyboard::Key _key, bool _pressed)
{
	keys[_key] = _pressed;
}

void InputManager::update()
{
	for (int key = 0; key < sf::Keyboard::KeyCount; key++)
		previousKeys[key] = keys[key];
}

bool InputManager::isKeyDown(sf::Keyboard::Key _key)
{
	return keys[_key];
}

bool InputManager::isKeyPressed(sf::Keyboard::Key _key)
{
	return (keys[_key] == true && previousKeys[_key] == false);
}

bool InputManager::isKeyReleased(sf::Keyboard::Key _key)
{
	return (keys[_key] == false && previousKeys[_key] == true);
}
