#include <string>
#include <iostream>

#include "../Headers/Game.hpp"


/// <summary>
/// Initialzer for the Game class
/// Initializes the window, renderer, and input controller, and sets the game up.
/// </summary>
Game::Game() : m_window(sf::VideoMode(totalWidth, totalHeight), "TETRIS"), m_renderer(&m_window), m_inputController(InputController(&m_window)) {
	for (u8 i = 0; i < numPlayers; ++i) {
		m_states[i] = std::make_unique<State>(State());
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
		m_playerColors[i] = pc;
		m_times[i] = sf::milliseconds(m_timeToNextDrop * 1000);
	}
	std::fill(m_board.begin(), m_board.end(), 0);
	m_musicController.startMusic();
	updateLevel();
	loop();
}

/// <summary>
/// Generates a new piece for the player(s) that have had their piece collide with the board
/// </summary>
/// <param name="playerIndex">The current player that needs a new piece</param>
void Game::newPiece(u8 playerIndex) {
	m_states[playerIndex]->piece = std::make_unique<Piece>(m_blocks.getBlock());

	m_states[playerIndex]->xOffset = ((gameWidth * (playerIndex + 1)) / numPlayers) - 4;
	m_states[playerIndex]->yOffset = 0;
	m_states[playerIndex]->rotation = 0;
}

/// <summary>
/// Gets the data at the current position, doing some math to turn a 1d vector into a 2d vector
/// </summary>
/// <param name="x">The x position of the piece</param>
/// <param name="y">The y position of the piece</param>
/// <param name="p">The piece</param>
/// <param name="rotation">The piece's rotation, or the rotation to check if checking for rotation validity</param>
/// <returns>Returns false if the data is 0, true if data is greater than 0</returns>
bool Game::getPiece(u8 x, u8 y, std::unique_ptr<Piece>& p, u8 rotation) {
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
/// The main game loop. Calls all necessary functions depending on the state of the game
/// </summary>
void Game::loop() {
	while (!quit && m_window.isOpen()) {
		input();
		renderGame();
		sf::Time timeDiff = m_clock.restart();
		for (u8 playerIndex = 0; playerIndex < numPlayers; ++playerIndex) {
			if (m_times[playerIndex].asMilliseconds() <= 0) {
				if (hasCollided(playerIndex)) {
					updateBoard(playerIndex);
					clearLines();
					quit = hasLost();
					updateLevel();
					newPiece(playerIndex);
				}
				else {
					++m_states[playerIndex]->yOffset;
				}
				setTimeNextDrop(playerIndex);
			}
			else {
				m_times[playerIndex] -= timeDiff;
			}
		}
	}
	while (quit && m_window.isOpen()) {
		m_renderer.clearRenderer();
		input();
		renderBoard();
		renderBorder();
		renderText();
		m_renderer.showRenderer();
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
	m_times[playerIndex] = sf::milliseconds(m_timeToNextDrop * 1000);
}

/// <summary>
/// Adds the dropped piece to the board. Uses the player index in order to maintain piece color
/// </summary>
/// <param name="playerIndex">The current player</param>
void Game::updateBoard(u8 playerIndex) { //Board gets updated when the playing piece collides with the m_board
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (bY >= 0)
					m_board[bY * gameWidth + bX] = playerIndex + 1;
				else {
					quit = true;
				}
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
	for (int x = 0; x < gameWidth; ++x) { //Every tile in a given row
		if (!m_board[y * gameWidth + x])//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}

/// <summary>
/// Clears any lines that are full and moves the rows above it down.
/// </summary>
void Game::clearLines() {
	m_clearedLines = 0;
	for (int y = 0; y < gameHeight; ++y) { //Every row
		if (isFullRow(y)) {
			for (int x = 0; x < gameWidth; ++x) {
				m_board[y * gameWidth + x] = 0; //If its a full row, we clear it
			}
			++m_clearedLines;
			m_yClearLevel = y;
		}
	}
	m_lines += m_clearedLines; //Updating total amount of lines cleared
	while (m_clearedLines > 0) { //If we clear 1 line, we drop the rest of the m_board one time. Two times for 2 cleared lines, and so on
		for (int y = m_yClearLevel; y > 0; --y) {
			for (int x = 0; x < gameWidth; ++x) {
				m_board[y * gameWidth + x] = m_board[(y - 1) * gameWidth + x];
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
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y + 1; //Checking the spot below the piece
				if (bY >= 0 && (m_board[bY * gameWidth + bX] || bY >= gameHeight)) {
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
	for (int x = 0; x < gameWidth; ++x) {
		if (m_board[x]) {//If there is a piece on the top row, then we lose
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
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (m_board[bY * gameWidth + bX]) {
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
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (bX < 0) {
					state->xOffset -= bX;
				}
				if (bX >= gameWidth) {
					state->xOffset -= (bX - gameWidth) + 1;
				}
				if (bY >= gameHeight) {
					--state->yOffset;
				}
			}
		}
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
	u8 finalAmount = gameHeight;
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				checkAmount = 0;
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y + 1;
				while (!m_board[bY * gameWidth + bX] && bY < gameHeight) {
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
/// The main function to call all child functions responsible for sending data to the renderer. 
/// </summary>
void Game::renderGame() {
	m_renderer.clearRenderer();

	for (u8 playerIndex = 0; playerIndex < numPlayers; ++playerIndex) {
		renderPiece(PieceToDraw::NormalPiece, playerIndex);
		u8 ghostPieceDrop = getBottom(playerIndex);
		renderPiece(PieceToDraw::GhostPiece, playerIndex, ghostPieceDrop);
	}

	renderBoard();
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
	std::unique_ptr<State>& state = m_states[playerIndex];
	PlayerColor* playerColor = &m_playerColors[playerIndex];
	for (int y = 0; y < state->piece->width; ++y) {
		for (int x = 0; x < state->piece->width; ++x) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				switch (piece) {
				case PieceToDraw::NormalPiece:
					m_renderer.draw(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset, playerColor->fillColor, sf::Color::White);
					break;
				case PieceToDraw::GhostPiece:
					m_renderer.draw(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset + ghostPieceOffset, playerColor->ghostFillColor, ghostOutlineColor);
					break;
				}
			}
		}
	}
}

/// <summary>
/// Sends the board to the renderer to be drawn
/// </summary>
void Game::renderBoard() {
	for (int x = 0; x < gameWidth; ++x) {
		for (int y = 0; y < gameHeight; ++y) {
			if (m_board[y * gameWidth + x]) {
				PlayerColor* playerColor = &m_playerColors[m_board[y * gameWidth + x] - 1];
				m_renderer.draw(x + boardXOffset, y + boardYOffset, playerColor->fillColor, sf::Color::White);
			}
		}
	}
}

/// <summary>
/// Sends the border around the playable area to be drawn
/// </summary>
void Game::renderBorder() {
	for (int x = 7; x < gameWidth + 9; ++x) {
		m_renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(x, gameHeight + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < gameHeight + 1; ++y) {
		m_renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(gameWidth + 8, y, sf::Color::White, sf::Color::Blue);
	}
}

/// <summary>
/// Sends the level information and the lines information to the renderer to be displayed
/// </summary>
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(m_level + 1); //Levels are 1-30 but arrays are 0-indexed, so add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(m_lines);
	m_renderer.drawText(totalWidth - 200, totalHeight / 2, lvlStr);
	m_renderer.drawText(totalWidth - 200, totalHeight / 2 + 50, linesStr);
}

/// <summary>
/// Checks if a given move is valid. Does not allow player to move outside the board, or into another piece on the board.
/// </summary>
/// <param name="move">The move needing to be checked</param>
/// <param name="playerIndex">The player making the move</param>
/// <returns></returns>
bool Game::isValidMove(Move move, u8 playerIndex) { //Can't move into the m_board or another piece
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;

				if ((bX <= 0 || m_board[(bY * gameWidth + bX - 1)]) && move == Move::Left)
					return false;
				if ((bX >= gameWidth - 1 || m_board[(bY * gameWidth + bX + 1)]) && move == Move::Right)
					return false;
				if (bY >= gameHeight && move == Move::Down)
					return false;
			}
		}
	}
	return true;
}

/// <summary>
/// Method used for hard dropping a piece
/// </summary>
/// <param name="playerIndex">The player dropping the piece</param>
void Game::dropPiece(u8 playerIndex) {
	m_states[playerIndex]->yOffset += getBottom(playerIndex);
	m_times[playerIndex] = sf::milliseconds(0);
}

/// <summary>
/// Gets the input from the input controller and updates the game accordingly
/// </summary>
void Game::input() {
	PlayerMove pm = m_inputController.input();
	switch (pm.move) {
	case Move::Right:
		if (isValidMove(Move::Right, pm.player))
			++m_states[pm.player]->xOffset;
		break;

	case Move::Left:
		if (isValidMove(Move::Left, pm.player))
			--m_states[pm.player]->xOffset;
		break;

	case Move::Down:
		if (isValidMove(Move::Down, pm.player))
			m_times[pm.player] = sf::milliseconds(0);
		break;

	case Move::Rotate:
		if (canWallKick((m_states[pm.player]->rotation + 1) % 4, pm.player)) {
			m_states[pm.player]->rotation = (m_states[pm.player]->rotation + 1) % 4;
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
/// Resets the game to the starting point. Can only be called after player(s) lost.
/// </summary>
void Game::restart() {
	quit = false;
	m_lines = 0;
	m_clearedLines = 0;
	m_level = 0;
	m_yClearLevel = 0;
	m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;
	for (u8 i = 0; i < numPlayers; ++i) {
		m_states[i]->piece.reset();
	}
	for (u8 i = 0; i < numPlayers; ++i) {
		newPiece(i);
		m_times[i] = sf::milliseconds(m_timeToNextDrop * 1000);
	}
	std::fill(m_board.begin(), m_board.end(), 0);
	m_musicController.startMusic();
	updateLevel();
	loop();
}