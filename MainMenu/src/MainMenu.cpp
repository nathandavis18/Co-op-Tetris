#include "../Headers/MainMenu.hpp"

MainMenu::MainMenu() : m_numPlayers(1), window(sf::VideoMode(mainMenuWindowWidth, mainMenuWindowHeight), "TETRIS"), m_eventHandler(&window) {
	showMainMenu();
}

void MainMenu::showMainMenu() {
	while (window.isOpen()) {
		switch (m_eventHandler.handleInput()) {
		case MainMenuAction::StartGame:
			setNumPlayers(m_numPlayers);
			startGame();
		}

	}
}

void MainMenu::setNumPlayers(u8 numPlayers) {
	m_numPlayers = numPlayers;
}

void MainMenu::setPlayerControl(u8 player, u8 controllerType, u8 input, u8 moveToMake) {
	return;
}

void MainMenu::calculateGameSizes() {
	gameWidth = m_baseWidth + ((m_numPlayers - 1) * m_scalingFactor);
	gameWindowWidth = (sideBuffer * 2 + gameWidth) * pieceSize;
	gameWindowHeight = (gameHeight + 2) * pieceSize;

	boardXOffset = (gameWindowWidth / pieceSize - gameWidth) / 2;
	boardYOffset = (gameWindowHeight / pieceSize - gameHeight) / 2;
}

void MainMenu::startGame() {
	calculateGameSizes();
	window.close();
	sf::RenderWindow gameWindow = sf::RenderWindow(sf::VideoMode(gameWindowWidth, gameWindowHeight), "TETRIS");
	Renderer mainRenderer = Renderer(pieceSize, &gameWindow);
	Board mainBoard = Board(gameWidth, boardHeight);
	InputController mainInputController = InputController(&gameWindow);
	MusicController mainMusicController;
	PieceState mainPieceState;
	Game game(m_numPlayers, gameWidth, gameHeight, boardXOffset, boardYOffset, gameWindowWidth, gameWindowHeight, &gameWindow, &mainRenderer, &mainBoard, &mainInputController, &mainMusicController, &mainPieceState);
}
