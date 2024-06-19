#include "../Headers/MainMenu.hpp"

MainMenu::MainMenu() : m_numPlayers(1), window(sf::VideoMode(mainMenuWindowWidth, mainMenuWindowHeight), "TETRIS"), m_eventHandler(&window) {
	window.clear();
	window.display();
	showMainMenu();
}

void MainMenu::showMainMenu() {
	while (window.isOpen()) {
		switch (m_eventHandler.handleInput()) {
		case MainMenuAction::StartGame:
			startGame();
			break;
		case MainMenuAction::ChangeControl:
			break;
		case MainMenuAction::ChangeNumPlayers:
			setNumPlayers(4);
			break;
		default:
			break;
		}

	}
}

void MainMenu::setNumPlayers(const u8 numPlayers) {
	m_numPlayers = numPlayers;
}

void MainMenu::setPlayerControl(const u8 player, const u8 controllerType, const u8 input, const u8 moveToMake) {
	return;
}

void MainMenu::calculateGameSizes() {
	gameWidth = m_baseWidth + ((m_numPlayers - 1) * m_scalingFactor);
	gameWindowWidth = (sideBuffer * 2 + gameWidth) * pieceSize;
	gameWindowHeight = (verticalBuffer * 2 + gameHeight + 2) * pieceSize;
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
	Blocks mainBlockGenerator;
	Game game(m_numPlayers, gameWidth, gameHeight, sideBuffer, verticalBuffer, gameWindowWidth, gameWindowHeight, &gameWindow, &mainRenderer, &mainBoard, &mainInputController, &mainMusicController, &mainPieceState, &mainBlockGenerator);
}
