#include <string>
#include <iostream>

#include "../Headers/Game.hpp"


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
	getLevel();
	loop();
}
void Game::newPiece(u8 playerIndex) {
	m_states[playerIndex]->piece = std::make_unique<Piece>(m_blocks.getBlock());

	m_states[playerIndex]->xOffset = ((gameWidth * (playerIndex + 1)) / numPlayers) - 4;
	m_states[playerIndex]->yOffset = 0;
	m_states[playerIndex]->rotation = 0;
}
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
					getLevel();
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
void Game::getLevel() {
	m_level = m_lines / 10;
	if (m_level > 29) {
		m_level = 29; //Level 29 (30 if not 0 indexing) is the max m_level
	}
}
void Game::setTimeNextDrop(u8 playerIndex) {
	m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;
	m_times[playerIndex] = sf::milliseconds(m_timeToNextDrop * 1000);
}
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
bool Game::isFullRow(int y) {
	for (int x = 0; x < gameWidth; ++x) { //Every tile in a given row
		if (!m_board[y * gameWidth + x])//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}
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
bool Game::hasLost() {
	for (int x = 0; x < gameWidth; ++x) {
		if (m_board[x]) {//If there is a piece on the top row, then we lose
			m_musicController.stopMusic();
			return true;
		}
	}
	return false;
}

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
//Sends everything to the renderer to be drawn
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
//Draws the piece you are playing, the ghost piece for hard-drop accuracy, and the upcoming piece
void Game::renderPiece(PieceToDraw piece, u8 playerIndex, u8 yAmount) {
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
					m_renderer.draw(x + state->xOffset + boardXOffset, y + state->yOffset + boardYOffset + yAmount, playerColor->ghostFillColor, ghostOutlineColor);
					break;
				}
			}
		}
	}
}
void Game::renderBoard() { //Draws all pieces that are currently on the m_board

	for (int x = 0; x < gameWidth; ++x) {
		for (int y = 0; y < gameHeight; ++y) {
			if (m_board[y * gameWidth + x]) {
				PlayerColor* playerColor = &m_playerColors[m_board[y * gameWidth + x] - 1];
				m_renderer.draw(x + boardXOffset, y + boardYOffset, playerColor->fillColor, sf::Color::White);
			}
		}
	}
}
void Game::renderBorder() { //Draws the border around the playable area
	for (int x = 7; x < gameWidth + 9; ++x) {
		m_renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(x, gameHeight + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < gameHeight + 1; ++y) {
		m_renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(gameWidth + 8, y, sf::Color::White, sf::Color::Blue);
	}
}
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(m_level + 1); //Levels are 1-30 but arrays are 0-indexed, so we add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(m_lines);
	m_renderer.drawText(totalWidth - 200, totalHeight / 2, lvlStr);
	m_renderer.drawText(totalWidth - 200, totalHeight / 2 + 50, linesStr);
}

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
void Game::dropPiece(u8 playerIndex) {
	m_states[playerIndex]->yOffset += getBottom(playerIndex);
	m_times[playerIndex] = sf::milliseconds(0);
}
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
	getLevel();
	loop();
}