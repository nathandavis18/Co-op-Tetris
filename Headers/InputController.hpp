#pragma once
#include <SFML/Graphics.hpp>

#include "Globals.hpp"

/// <summary>
/// This class abstracts the sf::Event input handler away from the Game class
/// </summary>
class InputController {
public:
	InputController(sf::Window*);
	PlayerMove input();
private:
	sf::Event m_event;
	sf::Window* m_window;
};