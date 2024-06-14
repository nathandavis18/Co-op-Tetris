#include <string>

#include "../Headers/Game.hpp"


Game::Game() : m_window(sf::VideoMode(width, height), "TETRIS"), m_renderer(&m_window) {
	for (u8 i = 0; i < numPlayers; ++i) {
		m_states[i] = std::make_unique<State>(State());
		m_states[i]->nextPiece = std::make_unique<Piece>(m_blocks.getBlock());
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
	}
	std::fill(m_board.begin(), m_board.end(), 0);
	m_time = sf::milliseconds(m_timeToNextDrop * 1000);
	getLevel();
	loop();
}
void Game::newPiece(u8 playerIndex) {
	m_states[playerIndex]->piece = std::move(m_states[playerIndex]->nextPiece); //Next piece becomes current piece
	m_states[playerIndex]->nextPiece = std::make_unique<Piece>(m_blocks.getBlock()); //Next piece becomes a new piece

	m_states[playerIndex]->xOffset = ((game_width * (playerIndex + 1)) / numPlayers) - 4;
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
	while (m_window.isOpen() && !quit) {
		input();
		renderGame();
		if (m_time.asMilliseconds() <= 0) {
			for (u8 i = 0; i < numPlayers; ++i) {
				++m_states[i]->yOffset;
			}
			setTimeNextDrop();
		}
		for (u8 playerIndex = 0; playerIndex < numPlayers; ++playerIndex) {
			if (hasCollided(playerIndex)) {
				--m_states[playerIndex]->yOffset;
				updateBoard(playerIndex);
				clearLines();
				quit = hasLost();
				getLevel();
				newPiece(playerIndex);
			}
		}

		m_time -= m_clock.restart();
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
void Game::setTimeNextDrop() {
	m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;
	m_time = sf::milliseconds(m_timeToNextDrop * 1000);
}
void Game::updateBoard(u8 playerIndex) { //Board gets updated when the playing piece collides with the m_board
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y;
				if (bY >= 0)
					m_board[bY * game_width + bX] = playerIndex + 1;
				else {
					quit = true;
				}
			}
		}
	}
}
bool Game::isFullRow(int y) {
	for (int x = 0; x < game_width; ++x) { //Every tile in a given row
		if (!m_board[y * game_width + x])//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}
void Game::clearLines() {
	m_clearedLines = 0;
	for (int y = 0; y < game_height; ++y) { //Every row
		if (isFullRow(y)) {
			for (int x = 0; x < game_width; ++x) {
				m_board[y * game_width + x] = 0; //If its a full row, we clear it
			}
			++m_clearedLines;
			m_yClearLevel = y;
		}
	}
	m_lines += m_clearedLines; //Updating total amount of lines cleared
	while (m_clearedLines > 0) { //If we clear 1 line, we drop the rest of the m_board one time. Two times for 2 cleared lines, and so on
		for (int y = m_yClearLevel; y > 0; --y) {
			for (int x = 0; x < game_width; ++x) {
				m_board[y * game_width + x] = m_board[(y - 1) * game_width + x];
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
				u8 bY = state->yOffset + y;
				if (bY >= 0 && (m_board[bY * game_width + bX] >= 1 || bY >= game_height)) {
					return true;
				}
			}
		}
	}
	return false;
}
bool Game::hasLost() {
	for (int x = 0; x < game_width; ++x) {
		if (m_board[x]) //If there is a piece on the top row, then we lose
			return true;
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
				if (m_board[bY * game_width + bX]) {
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
				if (bX >= game_width) {
					state->xOffset -= (bX - game_width) + 1;
				}
				if (bY >= game_height) {
					--state->yOffset;
				}
			}
		}
	}
}

u8 Game::getBottom(u8 playerIndex, bool ghost) {
	u8 checkAmount;
	u8 finalAmount = game_height;
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				checkAmount = 0;
				s8 bX = state->xOffset + x;
				s8 bY = state->yOffset + y + ghost; //If we are getting the ghost piece, it is 1 value higher
				while (m_board[bY * game_width + bX] < 1 && bY < game_height) {
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
		u8 ghostPieceDrop = getBottom(playerIndex, true);
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

	for (int x = 0; x < game_width; ++x) {
		for (int y = 0; y < game_height; ++y) {
			if (m_board[y * game_width + x]) {
				PlayerColor* playerColor = &m_playerColors[m_board[y * game_width + x] - 1];
				m_renderer.draw(x + boardXOffset, y + boardYOffset, playerColor->fillColor, sf::Color::White);
			}
		}
	}
}
void Game::renderBorder() { //Draws the border around the playable area
	for (int x = 7; x < game_width + 9; ++x) {
		m_renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(x, game_height + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < game_height + 1; ++y) {
		m_renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		m_renderer.draw(game_width + 8, y, sf::Color::White, sf::Color::Blue);
	}
}
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(m_level + 1); //Levels are 1-30 but arrays are 0-indexed, so we add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(m_lines);
	m_renderer.drawText(width - 200, height / 2, lvlStr);
	m_renderer.drawText(width - 200, height / 2 + 50, linesStr);
}

bool Game::isValidMove(Move move, u8 playerIndex) { //Can't move into the m_board or another piece
	std::unique_ptr<State>& state = m_states[playerIndex];
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece, state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;

				if ((bX <= 0 || m_board[(bY * game_width + bX - 1)]) && move == Move::Left)
					return false;
				if ((bX >= game_width - 1 || m_board[(bY * game_width + bX + 1)]) && move == Move::Right)
					return false;
				if (bY >= game_height && move == Move::Down)
					return false;
			}
		}
	}
	return true;
}
void Game::input() {
	while (m_window.pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::EventType::KeyPressed: 

			/**
			* Player One Controls
			*/
			if (event.key.code == sf::Keyboard::Right) {
				if(isValidMove(Move::Right, playerOne))
					++m_states[playerOne]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Left) {
				if(isValidMove(Move::Left, playerOne))
					--m_states[playerOne]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Down) {
				if (isValidMove(Move::Down, playerOne))
					++m_states[playerOne]->yOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Up) {
				if (canWallKick((m_states[playerOne]->rotation + 1) % 4, playerOne)) {
					m_states[playerOne]->rotation = (m_states[playerOne]->rotation + 1) % 4;
						wallKick(playerOne);
				}
				break;
			}
			if (event.key.code == sf::Keyboard::PageDown) {
				m_states[playerOne]->yOffset += getBottom(playerOne);
				break;
			}

			/**
			* Player Two Controls
			*/
			if (numPlayers >= 2) {
				if (event.key.code == sf::Keyboard::D) {
					if (isValidMove(Move::Right, playerTwo))
						++m_states[playerTwo]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::A) {
					if (isValidMove(Move::Left, playerTwo))
						--m_states[playerTwo]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::S) {
					if (isValidMove(Move::Down, playerTwo))
						++m_states[playerTwo]->yOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::W) {
					if (canWallKick((m_states[playerTwo]->rotation + 1) % 4, playerTwo)) {
						m_states[playerTwo]->rotation = (m_states[playerTwo]->rotation + 1) % 4;
						wallKick(playerTwo);
					}
					break;
				}
				if (event.key.code == sf::Keyboard::R) {
					m_states[playerTwo]->yOffset += getBottom(playerTwo);
					break;
				}
			}

			/**
			* Player Three Controls
			*/
			if (numPlayers >= 3) {
				if (event.key.code == sf::Keyboard::L) {
					if (isValidMove(Move::Right, playerThree))
						++m_states[playerThree]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::J) {
					if (isValidMove(Move::Left, playerThree))
						--m_states[playerThree]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::K) {
					if (isValidMove(Move::Down, playerThree))
						++m_states[playerThree]->yOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::I) {
					if (canWallKick((m_states[playerThree]->rotation + 1) % 4, playerThree)) {
						m_states[playerThree]->rotation = (m_states[playerThree]->rotation + 1) % 4;
						wallKick(playerThree);
					}
					break;
				}
				if (event.key.code == sf::Keyboard::P) {
					m_states[playerThree]->yOffset += getBottom(playerThree);
					break;
				}
			}

			/**
			* Player Four Controls
			*/
			if (numPlayers == 4) {
				if (event.key.code == sf::Keyboard::Numpad6) {
					if (isValidMove(Move::Right, playerFour))
						++m_states[playerFour]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::Numpad4) {
					if (isValidMove(Move::Left, playerFour))
						--m_states[playerFour]->xOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::Numpad5) {
					if (isValidMove(Move::Down, playerFour))
						++m_states[playerFour]->yOffset;
					break;
				}
				if (event.key.code == sf::Keyboard::Numpad8) {
					if (canWallKick((m_states[playerFour]->rotation + 1) % 4, playerFour)) {
						m_states[playerFour]->rotation = (m_states[playerFour]->rotation + 1) % 4;
						wallKick(playerFour);
					}
					break;
				}
				if (event.key.code == sf::Keyboard::Add) {
					m_states[playerFour]->yOffset += getBottom(playerFour);
					break;
				}
			}
			if (quit) {
				if (event.key.code == sf::Keyboard::F5) {
					restart();
				}
			}
		}
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
		m_states[i]->piece.get_deleter();
		m_states[i]->nextPiece.get_deleter();
	}
	for (u8 i = 0; i < numPlayers; ++i) {
		m_states[i]->nextPiece = std::make_unique<Piece>(m_blocks.getBlock());
		newPiece(i);
	}
	std::fill(m_board.begin(), m_board.end(), 0);
	m_time = sf::milliseconds(m_timeToNextDrop * 1000);
	getLevel();
	loop();
}