#include "../Headers/MainMenuEventHandler.hpp"
#include <iostream>

MainMenuEventHandler::MainMenuEventHandler(sf::Window* const window) : m_window(window), m_event(sf::Event()) {}

uint8_t MainMenuEventHandler::handleInput() {
	MainMenuAction mma = MainMenuAction::None;
	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			break;

		case sf::Event::EventType::KeyPressed:
			switch (m_event.key.code) {
			case sf::Keyboard::Num1:
				return 1;
			case sf::Keyboard::Numpad1:
				return 1;
			case sf::Keyboard::Num2:
				return 2;
			case sf::Keyboard::Numpad2:
				return 2;
			case sf::Keyboard::Num3:
				return 3;
			case sf::Keyboard::Numpad3:
				return 3;
			case sf::Keyboard::Numpad4:
				return 4;
			case sf::Keyboard::Num4:
				return 4;
			default:
				return 0;
			}
		default:
			return 0;
		}
	}
}