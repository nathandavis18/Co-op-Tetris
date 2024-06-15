#include "../Headers/InputController.hpp"

#include <iostream>
/// <summary>
/// Constructor to initialize the sf::Window pointer to the window address
/// Default initializes the event for handling input
/// </summary>
/// <param name="window">A pointer to the main window</param>
InputController::InputController(sf::Window* window) : m_window(window), m_event(sf::Event()) {}

/// <summary>
/// Detects which input was made and sets the move and player variables accordingly.
/// This method completely abstracts the sf::Event information away from the Game class
/// </summary>
/// <returns>Returns the move to make and which player made the move</returns>
PlayerMove InputController::input() {
	PlayerMove pm;
	pm.move = Move::None;

	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			return pm;
		case sf::Event::EventType::KeyPressed:
			/**
			* Player One Controls
			*/
			if (m_event.key.code == sf::Keyboard::Right) {
				pm.move = Move::Right;
				pm.player = playerOne;
				return pm;
			}
			if (m_event.key.code == sf::Keyboard::Left) {
				pm.move = Move::Left;
				pm.player = playerOne;
				return pm;
			}
			if (m_event.key.code == sf::Keyboard::Down) {
				pm.move = Move::Down;
				pm.player = playerOne;
				return pm;
			}
			if (m_event.key.code == sf::Keyboard::Up) {
				pm.move = Move::Rotate;
				pm.player = playerOne;
				return pm;
			}
			if (numPlayers == 1) { //If playing in single player mode, use Space for convenience. Otherwise, use Page Down to conserve space on the keyboard
				if (m_event.key.code == sf::Keyboard::Space) {
					pm.move = Move::HardDrop;
					pm.player = playerOne;
					return pm;
				}
			}
			else {
				if (m_event.key.code == sf::Keyboard::PageDown) {
					pm.move = Move::HardDrop;
					pm.player = playerOne;
					return pm;
				}
			}

			/**
			* Player Two Controls
			*/
			if (numPlayers >= 2) { //These controls should only be active if there are 2+ players
				if (m_event.key.code == sf::Keyboard::D) {
					pm.move = Move::Right;
					pm.player = playerTwo;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::A) {
					pm.move = Move::Left;
					pm.player = playerTwo;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::S) {
					pm.move = Move::Down;
					pm.player = playerTwo;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::W) {
					pm.move = Move::Rotate;
					pm.player = playerTwo;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::R) {
					pm.move = Move::HardDrop;
					pm.player = playerTwo;
					return pm;
				}
			}

			/**
			* Player Three Controls
			*/
			if (numPlayers >= 3) { // These controls should only be active if there are 3+ players
				if (m_event.key.code == sf::Keyboard::L) {
					pm.move = Move::Right;
					pm.player = playerThree;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::J) {
					pm.move = Move::Left;
					pm.player = playerThree;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::K) {
					pm.move = Move::Down;
					pm.player = playerThree;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::I) {
					pm.move = Move::Rotate;
					pm.player = playerThree;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::P) {
					pm.move = Move::HardDrop;
					pm.player = playerThree;
					return pm;
				}
			}

			/**
			* Player Four Controls
			*/
			if (numPlayers == 4) { // These controls should only be active if there are 4 players
				if (m_event.key.code == sf::Keyboard::Numpad6) {
					pm.move = Move::Right;
					pm.player = playerFour;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::Numpad4) {
					pm.move = Move::Left;
					pm.player = playerFour;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::Numpad5) {
					pm.move = Move::Down;
					pm.player = playerFour;
				}
				if (m_event.key.code == sf::Keyboard::Numpad8) {
					pm.move = Move::Rotate;
					pm.player = playerFour;
					return pm;
				}
				if (m_event.key.code == sf::Keyboard::Add) {
					pm.move = Move::HardDrop;
					pm.player = playerFour;
					return pm;
				}
			}

			if (quit) {
				if (m_event.key.code == sf::Keyboard::F5) {
					pm.move = Move::PlayAgain;
				}
			}
			return pm;

		/*
		* Having a controller for the 4th person might be easier.
		*/
		case sf::Event::EventType::JoystickMoved: //D-Pad falls under Joystick information rather than button information
			if (numPlayers == 4) {
				if (sf::Joystick::isConnected(0)) {
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == 100) {
						pm.move = Move::Right;
						pm.player = playerFour;
						return pm;
					}
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == -100) {
						pm.move = Move::Left;
						pm.player = playerFour;
						return pm;
					}
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) == -100) {
						pm.move = Move::Down;
						pm.player = playerFour;
						return pm;
					}
				}
			}
			return pm;

		case sf::Event::EventType::JoystickButtonPressed: // X/A on the controller
			if (numPlayers == 4) {
				if (sf::Joystick::isConnected(0)) {
					if (sf::Joystick::isButtonPressed(0, 3)) {
						pm.move = Move::Rotate;
						pm.player = playerFour;
						return pm;
					}
					if (sf::Joystick::isButtonPressed(0, 2)) {
						pm.move = Move::HardDrop;
						pm.player = playerFour;
						return pm;
					}
				}
			}
			return pm;

		default: // If no move was made
			pm.move = Move::None; 
			return pm;
		}

	}
	return pm;
}