#include <string>
#include <iostream>

#include "../Headers/Game.hpp"


/// <summary>
/// Initialzer for the Game class
/// Initializes the window, renderer, and input controller, and sets the game up.
/// </summary>
Game::Game(u8 numPlayers) : m_numPlayers(numPlayers), m_gameWidth(m_baseWidth + ((numPlayers - 1) * m_boardScalingFactor)), m_totalWidth((sideBuffer * 2 + m_gameWidth) * pieceSize),
m_totalHeight(pieceSize * (m_gameHeight + 2)), m_boardXOffset((m_totalWidth / pieceSize - m_gameWidth) / 2), m_boardYOffset((m_totalHeight / pieceSize - m_gameHeight) / 2),
m_renderer(&m_window), m_inputController(InputController(&m_window)), m_board(Board(m_gameWidth, m_gameHeight + 2)) {

	m_window.create(sf::VideoMode(m_totalWidth, m_totalHeight), "TETRIS");

	for (u8 i = 0; i < numPlayers; ++i) {
		m_pieceStates.push_back(std::make_unique<State>(State()));
		newPiece(i);

		PlayerColor pc;
		if (i == 0) {
			pc.fillColor = sf::Color::Red;
			pc.ghostFillColor = sf::Color(150, 0, 0, 100);
		}
		else if (i == 1) {
			pc.fillColor = sf::Color::Blue;
			pc.ghostFillColor = sf::Color(0, 0, 150, 100);
		}
		else if (i == 2) {
			pc.fillColor = sf::Color::Yellow;
			pc.ghostFillColor = sf::Color(150, 150, 0, 100);
		}
		else if (i == 3) {
			pc.fillColor = sf::Color::Magenta;
			pc.ghostFillColor = sf::Color(100, 0, 50, 100);
		}
		m_playerColors.push_back(pc);
		m_playerTimes.push_back(sf::milliseconds(m_timeToNextDrop * 1000));
	}
	m_musicController.startMusic();
	updateLevel();
	loop();
}

/// <summary>
/// The main game loop. Calls all necessary functions depending on the state of the game
/// </summary>
void Game::loop() {
	while (!m_quit && m_window.isOpen()) {
		input();
		renderGame();
		sf::Time timeDiff = m_clock.restart();
		for (u8 playerIndex = 0; playerIndex < m_numPlayers; ++playerIndex) {
			if (m_playerTimes[playerIndex].asMilliseconds() <= 0) {
				if (hasCollided(playerIndex)) {
					updateBoard(playerIndex);
					clearLines();
					m_quit = hasLost();
					updateLevel();
					newPiece(playerIndex);
				}
				else {
					++m_pieceStates[playerIndex]->yOffset;
				}
				setTimeNextDrop(playerIndex);
			}
			else {
				m_playerTimes[playerIndex] -= timeDiff;
			}
		}
	}
	while (m_quit && m_window.isOpen()) {
		m_renderer.clearRenderer();
		input();
		m_board.renderBoard(&m_renderer, m_playerColors, m_boardXOffset, m_boardYOffset);
		renderBorder();
		renderText();
		m_renderer.showRenderer();
	}
}

/// <summary>
/// Generates a new piece for the player(s) that have had their piece collide with the board
/// </summary>
/// <param name="playerIndex">The current player that needs a new piece</param>
void Game::newPiece(u8 playerIndex) {
	m_pieceStates[playerIndex]->piece = std::make_unique<Piece>(m_blockGenerator.getBlock());

	m_pieceStates[playerIndex]->xOffset = ((m_gameWidth * (playerIndex + 1)) / m_numPlayers) - 4;
	m_pieceStates[playerIndex]->yOffset = 0;
	m_pieceStates[playerIndex]->rotation = 0;
}

/// <summary>
/// Gets the data at the current position, doing some math to turn a 1d vector into a 2d vector
/// </summary>
/// <param name="x">The x position of the piece</param>
/// <param name="y">The y position of the piece</param>
/// <param name="p">The piece</param>
/// <param name="rotation">The piece's rotation, or the rotation to check if checking for rotation validity</param>
/// <returns>Returns false if the data is 0, true if data is greater than 0</returns>
bool Game::getPieceData(u8 x, u8 y, std::unique_ptr<Piece>& p, u8 rotation) {
	switch (rotation) {
	case 0:
		return (p->data[y * p->width + x]);
	case 1:
		return (p->data[(p->width - x - 1) * p->width + y]);
	case 2:
		return (p->data[(p->width - y - 1) * p->width + (p->width - x - 1)]);
	case 3:
		return (p->data[(p->width * x) + (p->width - y - 1)]);
	default:
		return 0;
	}
}

/// <summary>
/// Updates the level based on the amount of lines cleared
/// </summary>
void Game::updateLevel() {
	m_level = m_lines / 10;
	if (m_level > 29) {
		m_level = 29; //Level 29 (30 if not 0 indexing) is the max m_level
	}
}

/// <summary>
/// Sets the time for the current player for the next automatic drop of their piece
/// </summary>
/// <param name="playerIndex">The current player</param>
void Game::setTimeNextDrop(u8 playerIndex) {
	m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;
	m_playerTimes[playerIndex] = sf::milliseconds(m_timeToNextDrop * 1000);
}

/// <summary>
/// Adds the dropped piece to the board. Uses the player index in order to maintain piece color
/// </summary>
/// <param name="playerIndex">The current player</param>
void Game::updateBoard(u8 playerIndex) { //Board gets updated when the playing piece collides with the m_board
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;

				if (bY >= 0) m_board.setBoardPosition(bX, bY, playerIndex + 1);
				else m_quit = true;
			}
		}
	}
}

/// <summary>
/// Checks if the current row is full, starting from left to right.
/// Returns false if any piece in the row is not filled
/// </summary>
/// <param name="y">The row to check</param>
/// <returns></returns>
bool Game::isFullRow(int y) {
	for (int x = 0; x < m_gameWidth; ++x) { //Every tile in a given row
		if (!m_board.getBoardPosition(x, y))//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}

/// <summary>
/// Clears any lines that are full and moves the rows above it down.
/// </summary>
void Game::clearLines() {
	m_clearedLines = 0;
	for (int y = 0; y < m_gameHeight; ++y) { //Every row
		if (isFullRow(y)) {
			for (int x = 0; x < m_gameWidth; ++x) {
				m_board.setBoardPosition(x, y, 0); //If its a full row, we clear it
			}
			++m_clearedLines;
			m_yClearLevel = y;
		}
	}
	m_lines += m_clearedLines; //Updating total amount of lines cleared
	while (m_clearedLines > 0) { //If we clear 1 line, we drop the rest of the m_board one time. Two times for 2 cleared lines, and so on
		for (int y = m_yClearLevel; y > 0; --y) {
			for (int x = 0; x < m_gameWidth; ++x) {
				m_board.setBoardPosition(x, y, m_board.getBoardPosition(x, (y - 1)));
			}
		}
		--m_clearedLines;
	}
}

/// <summary>
/// Checks if the current player's piece has collided with the board.
/// </summary>
/// <param name="playerIndex">The current player</param>
/// <returns></returns>
bool Game::hasCollided(u8 playerIndex) {
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y + 1; //Checking the spot below the piece
				if (bY >= 0 && (m_board.getBoardPosition(bX, bY) || bY >= m_gameHeight)) {
					return true;
				}
			}
		}
	}
	return false;
}

/// <summary>
/// Checks if the top row contains a piece. If so, the player has lost.
/// This method is called after clearLines in the game loop.
/// </summary>
/// <returns></returns>
bool Game::hasLost() {
	for (int x = 0; x < m_gameWidth; ++x) {
		if (m_board.getBoardPosition(x, 0)) {//If there is a piece on the top row, then we lose
			m_musicController.stopMusic();
			return true;
		}
	}
	return false;
}

/// <summary>
/// Checks if when the player rotates the piece can be pushed or not by checking collision above and to the sides of it.
/// </summary>
/// <param name="rotation">The rotation to check</param>
/// <param name="playerIndex">The player's piece to check</param>
/// <returns></returns>
bool Game::canWallKick(u8 rotation, u8 playerIndex) {
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (m_board.getBoardPosition(bX, bY)) {
					return false;
				}
			}
		}
	}
	return true;
}

/// <summary>
/// Pushes the piece away from the wall if it can.
/// </summary>
/// <param name="playerIndex">The current player's piece</param>
void Game::wallKick(u8 playerIndex) {
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (bX < 0) {
					state->xOffset -= bX;
				}
				if (bX >= m_gameWidth) {
					state->xOffset -= (bX - m_gameWidth) + 1;
				}
				if (bY >= m_gameHeight) {
					--state->yOffset;
				}
			}
		}
	}
}

/// <summary>
/// Checks if a given move is valid. Does not allow player to move outside the board, or into another piece on the board.
/// </summary>
/// <param name="move">The move needing to be checked</param>
/// <param name="playerIndex">The player making the move</param>
/// <returns></returns>
bool Game::isValidMove(Move move, u8 playerIndex) { //Can't move into the m_board or another piece
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;

				if ((bX <= 0 || m_board.getBoardPosition(bX - 1, bY)) && move == Move::Left)
					return false;
				if ((bX >= m_gameWidth - 1 || m_board.getBoardPosition(bX + 1, bY)) && move == Move::Right)
					return false;
				if (bY >= m_gameHeight && move == Move::Down)
					return false;
			}
		}
	}
	return true;
}

/// <summary>
/// Gets the input from the input controller and updates the game accordingly
/// </summary>
void Game::input() {
	PlayerMove pm = m_inputController.input(m_quit);
	switch (pm.move) {
	case Move::Right:
		if (isValidMove(Move::Right, pm.player))
			++m_pieceStates[pm.player]->xOffset;
		break;

	case Move::Left:
		if (isValidMove(Move::Left, pm.player))
			--m_pieceStates[pm.player]->xOffset;
		break;

	case Move::Down:
		if (isValidMove(Move::Down, pm.player))
			m_playerTimes[pm.player] = sf::milliseconds(0);
		break;

	case Move::Rotate:
		if (canWallKick((m_pieceStates[pm.player]->rotation + 1) % 4, pm.player)) {
			m_pieceStates[pm.player]->rotation = (m_pieceStates[pm.player]->rotation + 1) % 4;
			wallKick(pm.player);
		}
		break;

	case Move::HardDrop:
		dropPiece(pm.player);
		break;

	case Move::PlayAgain:
		restart();
		break;

	default:
		break;
	}
}

/// <summary>
/// Finds the highest point of collision directly below the player's piece. 
/// Used for drawing the ghost piece and for hard drops
/// </summary>
/// <param name="playerIndex">The current player's piece</param>
/// <returns>The amount that the piece can go down</returns>
u8 Game::getBottom(u8 playerIndex) {
	u8 checkAmount;
	u8 finalAmount = m_gameHeight;
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				checkAmount = 0;
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y + 1;
				while (!m_board.getBoardPosition(bX, bY) && bY < m_gameHeight) {
					++checkAmount;
					++bY;
				}
				if (checkAmount < finalAmount) {
					finalAmount = checkAmount;
				}
			}
		}
	}
	return finalAmount;
}

/// <summary>
/// Method used for hard dropping a piece
/// </summary>
/// <param name="playerIndex">The player dropping the piece</param>
void Game::dropPiece(u8 playerIndex) {
	m_pieceStates[playerIndex]->yOffset += getBottom(playerIndex);
	m_playerTimes[playerIndex] = sf::milliseconds(0);
}

/// <summary>
/// The main function to call all child functions responsible for sending data to the renderer. 
/// </summary>
void Game::renderGame() {
	m_renderer.clearRenderer();

	for (u8 playerIndex = 0; playerIndex < m_numPlayers; ++playerIndex) {
		renderPiece(PieceToDraw::NormalPiece, playerIndex);
		u8 ghostPieceDrop = getBottom(playerIndex);
		renderPiece(PieceToDraw::GhostPiece, playerIndex, ghostPieceDrop);
	}

	m_board.renderBoard(&m_renderer, m_playerColors, m_boardXOffset, m_boardYOffset);
	renderBorder();
	renderText();

	m_renderer.showRenderer();
}

/// <summary>
/// Sends the player's current piece and the ghost piece associated with their piece to the renderer.
/// </summary>
/// <param name="piece">What type of piece needs to be drawn (NormalPiece/GhostPiece)</param>
/// <param name="playerIndex">The player's piece to draw</param>
/// <param name="ghostPieceOffset">The offset between the player's piece and their ghost piece</param>
void Game::renderPiece(PieceToDraw piece, u8 playerIndex, u8 ghostPieceOffset) {
	std::unique_ptr<State>& state = m_pieceStates[playerIndex];
	PlayerColor* playerColor = &m_playerColors[playerIndex];
	for (int y = 0; y < state->piece->width; ++y) {
		for (int x = 0; x < state->piece->width; ++x) {
			if (getPieceData(x, y, state->piece, state->rotation)) {
				switch (piece) {
				case PieceToDraw::NormalPiece:
					m_renderer.draw(x + state->xOffset + m_boardXOffset, y + state->yOffset + m_boardYOffset, playerColor->fillColor, sf::Color::White);
					break;
				case PieceToDraw::GhostPiece:
					m_renderer.draw(x + state->xOffset + m_boardXOffset, y + state->yOffset + m_boardYOffset + ghostPieceOffset, playerColor->ghostFillColor, ghostOutlineColor);
					break;
				}
			}
		}
	}
}

/// <summary>
/// Sends the border around the playable area to be drawn
/// </summary>
void Game::renderBorder() {
	for (int x = 7; x < m_gameWidth + 9; ++x) {
		m_renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(x, m_gameHeight + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < m_gameHeight + 1; ++y) {
		m_renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(m_gameWidth + 8, y, sf::Color::White, sf::Color::Blue);
	}
}

/// <summary>
/// Sends the level information and the lines information to the renderer to be displayed
/// </summary>
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(m_level + 1); //Levels are 1-30 but arrays are 0-indexed, so add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(m_lines);
	m_renderer.drawText(m_totalWidth - 200, m_totalHeight / 2, lvlStr);
	m_renderer.drawText(m_totalWidth - 200, m_totalHeight / 2 + 50, linesStr);
}

/// <summary>
/// Resets the game to the starting point. Can only be called after player(s) lost.
/// </summary>
void Game::restart() {
	m_quit = false;
	m_lines = 0;
	m_clearedLines = 0;
	m_level = 0;
	m_yClearLevel = 0;
	m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;
	for (u8 i = 0; i < m_numPlayers; ++i) {
		m_pieceStates[i]->piece.reset();
		newPiece(i);
		m_playerTimes[i] = sf::milliseconds(m_timeToNextDrop * 1000);
	}
	m_board.resetBoard();
	m_musicController.startMusic();
	updateLevel();
	loop();
}