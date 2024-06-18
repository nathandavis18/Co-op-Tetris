#include "../Headers/MainMenuEventHandler.hpp"
#include <iostream>

MainMenuEventHandler::MainMenuEventHandler(sf::Window* const window) : m_window(window), m_event(sf::Event()) {}

MainMenuAction MainMenuEventHandler::handleInput() {
	MainMenuAction mma = MainMenuAction::None;
	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			break;

		case sf::Event::EventType::MouseButtonPressed:
			if (m_event.mouseButton.button == sf::Mouse::Left) {
				mma = MainMenuAction::StartGame;
				break;
			}
			else if (m_event.mouseButton.button == sf::Mouse::Right) {
				mma = MainMenuAction::ChangeNumPlayers;
				break;
			}
			break;
		default:
			break;
		}
	}

	return mma;
}