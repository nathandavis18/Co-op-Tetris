#include "../Headers/InputController.hpp"
#include <iostream>
#include <filesystem>
#include <string>


/// <summary>
/// A constructor used to construct a keyboard control input
/// </summary>
/// <param name="playerIndex">The player index from the controls layout</param>
/// <param name="input">The keyboard key code</param>
/// <param name="moveToMake">What move this input is supposed to make</param>
InputController::PlayerKeyboardControls::PlayerKeyboardControls(u8 playerIndex, u8 input, u8 moveToMake) : playerIndex(playerIndex), keyboardInput(input), moveToMake(moveToMake) {}

/// <summary>
/// A constructor used to construct a joystick control input
/// </summary>
/// <param name="playerIndex">The player index from the controls layout</param>
/// <param name="input">The keyboard key code</param>
/// <param name="moveToMake">What move this input is supposed to make</param>
InputController::PlayerJoystickControls::PlayerJoystickControls(u8 playerIndex, u8 input, u8 moveToMake) : playerIndex(playerIndex), controllerInput(input), moveToMake(moveToMake) {}

/// <summary>
/// Constructor to initialize the sf::Window pointer to the window address
/// Default initializes the event for handling input
/// Reads the control layout from the controls.txt file, or sets the values to their default based on the default-controls.txt layout if no controls.txt file is found.
/// </summary>
/// <param name="window">A pointer to the main window</param>
InputController::InputController(sf::Window* window) : m_window(window), m_event(sf::Event()) {
	std::filesystem::path controlsPath = std::filesystem::current_path() / "../../../../Controls/controls.txt";
	std::filesystem::path defaultControlsPath = std::filesystem::current_path() / "../../../../Controls/default-controls.txt";
	if (!std::filesystem::exists(controlsPath)) {
		std::ifstream defaultControlsFile(defaultControlsPath);
		if (!defaultControlsFile.is_open()) {
			std::cerr << "No default controls file found. Exiting program" << std::endl;
			exit(EXIT_FAILURE);
		}
		else {
			std::ofstream outfile(controlsPath);
			outfile << defaultControlsFile.rdbuf();
			outfile.close();
		}
	}
	std::ifstream controls(controlsPath);
	if (!controls.is_open()) {
		std::cerr << "Error opening controls file. Exiting program" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string control;
	try { //Try to read the controls in. If there is an error, output it to the console and exit the program.
		while (std::getline(controls, control)) {
			u8 count = 0;
			u8 controlType, playerIndex, moveToMake, input;
			std::string delimiter = ",";
			size_t pos = 0;
			while ((pos = control.find(delimiter)) != std::string::npos) {
				std::string cur = control.substr(0, pos);
				if (count == 0) {
					playerIndex = std::stoi(cur);
				}
				if (count == 1) {
					controlType = std::stoi(cur);
				}
				if (count == 2) {
					input = std::stoi(control);
				}
				++count;
				control.erase(0, pos + delimiter.length());
			}
			moveToMake = std::stoi(control);
			switch (controlType) {
			case sf::Event::EventType::KeyPressed:
				m_playerKeyboardControls.push_back(PlayerKeyboardControls(playerIndex, input, moveToMake));
				break;
			case sf::Event::EventType::JoystickButtonPressed:
				m_playerJoystickControls.push_back(PlayerJoystickControls(playerIndex, input, moveToMake));
				break;
			case sf::Event::EventType::JoystickMoved:
				m_playerJoystickControls.push_back(PlayerJoystickControls(playerIndex, input, moveToMake));
				break;
			default: break;
			}
		}
	}
	catch (std::exception ex) {
		std::cerr << "Error reading controls: " << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}

/// <summary>
/// Detects which input was made and sets the move and player variables accordingly.
/// This method completely abstracts the sf::Event information away from the Game class
/// </summary>
/// <returns>Returns the move to make and which player made the move</returns>
PlayerMove InputController::input(bool quit) {
	PlayerMove pm;
	pm.move = Move::None;

	while (m_window->pollEvent(m_event)) {
		switch (m_event.type) {
		case sf::Event::Closed:
			m_window->close();
			return pm;

		case sf::Event::EventType::KeyPressed:
			if (quit) {
				if (m_event.key.code == sf::Keyboard::F5) {
					pm.move = Move::PlayAgain;
					return pm;
				}
			}

			for (auto x : m_playerKeyboardControls) {
				if (x.keyboardInput == m_event.key.code) {
					switch (x.moveToMake) {
					case Move::Right:
						pm.move = Move::Right;
						break;
					case Move::Left:
						pm.move = Move::Left;
						break;
					case Move::Down:
						pm.move = Move::Down;
						break;
					case Move::Rotate:
						pm.move = Move::Rotate;
						break;
					case Move::HardDrop:
						pm.move = Move::HardDrop;
						break;
					}
					pm.player = x.playerIndex;
					break;
				}
			}
			break;

		case sf::Event::EventType::JoystickButtonPressed:
			for (auto x : m_playerJoystickControls) {
				if (sf::Joystick::isButtonPressed(0, x.controllerInput)) {
					switch (x.moveToMake) {
					case Move::Right:
						pm.move = Move::Right;
						break;
					case Move::Left:
						pm.move = Move::Left;
						break;
					case Move::Down:
						pm.move = Move::Down;
						break;
					case Move::Rotate:
						pm.move = Move::Rotate;
						break;
					case Move::HardDrop:
						pm.move = Move::HardDrop;
						break;
					}
					pm.player = x.playerIndex;
					break;
				}
			}
			break;
		case sf::Event::EventType::JoystickMoved:
			for (auto x : m_playerJoystickControls) {
				if (m_event.joystickMove.axis == x.controllerInput && x.controllerInput == sf::Joystick::PovX) {
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == 100) {
						pm.move = Move::Right;
						pm.player = x.playerIndex;
						break;
					}
					else if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovX) == -100) {
						pm.move = Move::Left;
						pm.player = x.playerIndex;
						break;
					}
				}
				else if (m_event.joystickMove.axis == x.controllerInput && x.controllerInput == sf::Joystick::PovY) {
					if (sf::Joystick::getAxisPosition(0, sf::Joystick::PovY) == -100) {
						pm.move = Move::Down;
						pm.player = x.playerIndex;
						break;
					}
				}
				else {
					continue;
				}
			}
			break;

		default: // If no move was made
			break;
		}

	}
	return pm;
}