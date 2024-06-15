#pragma once
#include <SFML/Graphics.hpp>

#include "Globals.hpp"
class InputController {
public:
	InputController(sf::Window*);
	PlayerMove input();
private:
	sf::Event m_event;
	sf::Window* m_window;
};