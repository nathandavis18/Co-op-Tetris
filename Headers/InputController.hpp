#pragma once
#include <SFML/Window.hpp>
#include <fstream>

#include "Globals.hpp"

/// <summary>
/// This class abstracts the sf::Event input handler away from the Game class
/// </summary>
class InputController
{
public:
	InputController(sf::Window* const);
	PlayerMove input(const bool = false);
private:
	struct PlayerKeyboardControls
	{
		PlayerKeyboardControls(const u8 playerIndex, const u8 input, const u8 moveToMake);
		u8 playerIndex;
		u8 keyboardInput;
		u8 moveToMake;
	};
	struct PlayerJoystickControls
	{
		PlayerJoystickControls(const u8 playerIndex, const u8 input, const u8 moveToMake);
		u8 playerIndex;
		u8 controllerInput;
		u8 moveToMake;
	};
	std::vector<PlayerKeyboardControls> m_playerKeyboardControls;
	std::vector<PlayerJoystickControls> m_playerJoystickControls;

	sf::Event m_event;
	sf::Window* m_window;
};