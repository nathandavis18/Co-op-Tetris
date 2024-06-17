#include "../Headers/MainMenuEventHandler.hpp"

MainMenuEventHandler::MainMenuEventHandler(sf::Window* window) : m_window(window), m_event(sf::Event()) {}

MainMenuAction MainMenuEventHandler::handleInput() {
	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			break;

		case sf::Event::EventType::MouseButtonPressed:
			return MainMenuAction::StartGame;

		default:
			return MainMenuAction::None;
		}
	}
}