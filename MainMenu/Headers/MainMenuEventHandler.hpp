#pragma once
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

enum class MainMenuAction { ChangeNumPlayers, ChangeControl, StartGame, None };

class MainMenuEventHandler
{
public:
	MainMenuEventHandler(sf::Window* const);
	uint8_t handleInput();
private:
	sf::Event m_event;
	sf::Window* m_window;
};