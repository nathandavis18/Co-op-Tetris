#pragma once
#include <fstream>
#include <SFML/Graphics.hpp>

#include "../../Headers/Globals.hpp"
#include "../../Headers/Game.hpp"
#include "MainMenuEventHandler.hpp"

class MainMenu {
public:
	MainMenu();
	void showMainMenu();
	void setNumPlayers(u8 numPlayers);
	void setPlayerControl(u8 player, u8 controllerType, u8 input, u8 moveToMake);
	void calculateGameSizes();
	void startGame();
private:
	sf::RenderWindow window;
	MainMenuEventHandler m_eventHandler;
	MainMenuAction m_menuAction;

	static constexpr u8 m_baseWidth = 10;
	static constexpr double m_scalingFactor = 3.4;
	static constexpr u8 pieceSize = 30;
	static constexpr u8 gameHeight = 20;
	static constexpr u8 boardHeight = 22;

	static constexpr u16 mainMenuWindowHeight = 720;
	static constexpr u16 mainMenuWindowWidth = 1080;


	u8 m_numPlayers;
	u8 gameWidth;
	u16 gameWindowWidth, gameWindowHeight, boardXOffset, boardYOffset;
};