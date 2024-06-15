#include "../Headers/InputController.hpp"

InputController::InputController(sf::Window* window) : m_window(window), m_event(sf::Event()) {}

PlayerMove InputController::input() {
	PlayerMove pm;
	pm.move = Move::Default;

	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			break;
		case sf::Event::EventType::KeyPressed:
			/**
			* Player One Controls
			*/
			if (m_event.key.code == sf::Keyboard::Right) {
				pm.move = Move::Right;
				pm.player = playerOne;
				break;
			}
			if (m_event.key.code == sf::Keyboard::Left) {
				pm.move = Move::Left;
				pm.player = playerOne;
				break;
			}
			if (m_event.key.code == sf::Keyboard::Down) {
				pm.move = Move::Down;
				pm.player = playerOne;
				break;
			}
			if (m_event.key.code == sf::Keyboard::Up) {
				pm.move = Move::Rotate;
				pm.player = playerOne;
				break;
			}
			if (numPlayers == 1) {
				if (m_event.key.code == sf::Keyboard::Space) {
					pm.move = Move::HardDrop;
					pm.player = playerOne;
					break;
				}
			}
			else {
				if (m_event.key.code == sf::Keyboard::PageDown) {
					pm.move = Move::HardDrop;
					pm.player = playerOne;
					break;
				}
			}

			/**
			* Player Two Controls
			*/
			if (numPlayers >= 2) {
				if (m_event.key.code == sf::Keyboard::D) {
					pm.move = Move::Right;
					pm.player = playerTwo;
					break;
				}
				if (m_event.key.code == sf::Keyboard::A) {
					pm.move = Move::Left;
					pm.player = playerTwo;
					break;
				}
				if (m_event.key.code == sf::Keyboard::S) {
					pm.move = Move::Down;
					pm.player = playerTwo;
					break;
				}
				if (m_event.key.code == sf::Keyboard::W) {
					pm.move = Move::Rotate;
					pm.player = playerTwo;
					break;
				}
				if (m_event.key.code == sf::Keyboard::R) {
					pm.move = Move::HardDrop;
					pm.player = playerTwo;
					break;
				}
			}

			/**
			* Player Three Controls
			*/
			if (numPlayers >= 3) {
				if (m_event.key.code == sf::Keyboard::L) {
					pm.move = Move::Right;
					pm.player = playerThree;
					break;
				}
				if (m_event.key.code == sf::Keyboard::J) {
					pm.move = Move::Left;
					pm.player = playerThree;
					break;
				}
				if (m_event.key.code == sf::Keyboard::K) {
					pm.move = Move::Down;
					pm.player = playerThree;
					break;
				}
				if (m_event.key.code == sf::Keyboard::I) {
					pm.move = Move::Rotate;
					pm.player = playerThree;
					break;
				}
				if (m_event.key.code == sf::Keyboard::P) {
					pm.move = Move::HardDrop;
					pm.player = playerThree;
					break;
				}
			}

			/**
			* Player Four Controls
			*/
			if (numPlayers == 4) {
				if (m_event.key.code == sf::Keyboard::Numpad6) {
					pm.move = Move::Right;
					pm.player = playerFour;
					break;
				}
				if (m_event.key.code == sf::Keyboard::Numpad4) {
					pm.move = Move::Left;
					pm.player = playerFour;
					break;
				}
				if (m_event.key.code == sf::Keyboard::Numpad5) {
					pm.move = Move::Down;
					pm.player = playerFour;
				}
				if (m_event.key.code == sf::Keyboard::Numpad8) {
					pm.move = Move::Rotate;
					pm.player = playerFour;
					break;
				}
				if (m_event.key.code == sf::Keyboard::Add) {
					pm.move = Move::HardDrop;
					pm.player = playerFour;
					break;
				}
			}

			if (quit) {
				if (m_event.key.code == sf::Keyboard::F5) {
					pm.move = Move::PlayAgain;
				}
			}
			break;

			/*
			* Having a controller for the 4th person might be easier.
			*/
		case sf::Event::EventType::JoystickMoved:
			if (numPlayers == 4) {
				if (sf::Joystick::isConnected(0)) {
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == 100) {
						pm.move = Move::Right;
						pm.player = playerFour;
						break;
					}
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == -100) {
						pm.move = Move::Left;
						pm.player = playerFour;
						break;
					}
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) == -100) {
						pm.move = Move::Down;
						pm.player = playerFour;
						break;
					}
				}
			}
			break;

		case sf::Event::EventType::JoystickButtonPressed:
			if (numPlayers == 4) {
				if (sf::Joystick::isConnected(0)) {
					if (sf::Joystick::isButtonPressed(0, 3)) {
						pm.move = Move::Rotate;
						pm.player = playerFour;
						break;
					}
					if (sf::Joystick::isButtonPressed(0, 2)) {
						pm.move = Move::HardDrop;
						pm.player = playerFour;
						break;
					}
				}
			}
			break;

		default:
			break;
		}

	}
	return pm;
}