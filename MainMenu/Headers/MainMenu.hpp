#pragma once
#include <fstream>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../Headers/Globals.hpp"
#include "../../Headers/Game.hpp"
#include "MainMenuEventHandler.hpp"

class MainMenu
{
public:
	MainMenu();
	void showMainMenu();
	void renderMainMenu();
	void setNumPlayers(const u8 numPlayers);
	void setPlayerControl(const u8 player, const u8 controllerType, const u8 input, const u8 moveToMake);
	void calculateGameSizes();
	void startGame();
private:
	sf::RenderWindow window;
	MainMenuEventHandler m_eventHandler;
	MainMenuAction m_menuAction;

	static constexpr u8 m_baseWidth = 10;
	static constexpr double m_scalingFactor = 3.4;
	static constexpr u8 pieceSize = 28;
	static constexpr u8 gameHeight = 20;
	static constexpr u8 boardHeight = 22;

	static constexpr u16 mainMenuWindowHeight = 600;
	static constexpr u16 mainMenuWindowWidth = 600;


	u8 m_numPlayers;
	u8 gameWidth;
	u16 gameWindowWidth, gameWindowHeight;

	sf::Image bgImage;
	sf::Texture bgTexture;
	sf::Sprite bgSprite;
};