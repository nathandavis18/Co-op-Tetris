#include <string>

#include "../Headers/Game.hpp"


Game::Game() : m_window(sf::VideoMode(width, height), "TETRIS"), renderer(&m_window) {
	for (u8 i = 0; i < numPlayers; ++i) {
		states[i] = std::make_unique<State>(State());
		states[i]->nextPiece = std::make_unique<Piece>(blocks.getBlock());
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
		playerColors[i] = pc;
	}
	std::fill(board.begin(), board.end(), 0);
	time = sf::milliseconds(timeToNextDrop * 1000);
	getLevel();
	loop();
}
void Game::newPiece(u8 playerIndex) {
	states[playerIndex]->piece = std::move(states[playerIndex]->nextPiece); //Next piece becomes current piece
	states[playerIndex]->nextPiece = std::make_unique<Piece>(blocks.getBlock()); //Next piece becomes a new piece

	states[playerIndex]->xOffset = ((game_width * (playerIndex + 1)) / numPlayers) - 4;
	states[playerIndex]->yOffset = 0;
	states[playerIndex]->rotation = 0;
}
bool Game::getPiece(u8 x, u8 y, Piece* p, u8 rotation) {
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
		if (time.asMilliseconds() <= 0) {
			for (u8 i = 0; i < states.size(); ++i) {
				++states[i]->yOffset;
			}
			setTimeNextDrop();
		}
		for (u8 i = 0; i < states.size(); ++i) {
			if (hasCollided(states[i].get())) {
				--states[i]->yOffset;
				updateBoard(i);
				clearLines();
				quit = hasLost();
				getLevel();
				newPiece(i);
			}
		}

		time -= clock.restart();
	}
	while (quit && m_window.isOpen()) {
		renderer.clearRenderer();
		input();
		renderBoard();
		renderBorder();
		renderText();
		renderer.showRenderer();
	}
}
void Game::getLevel() {
	level = lines / 10;
	if (level > 29) {
		level = 29; //Level 29 (30 if not 0 indexing) is the max level
	}
}
void Game::setTimeNextDrop() {
	timeToNextDrop = framesPerDrop[level] / framesPerSecond;
	time = sf::milliseconds(timeToNextDrop * 1000);
}
void Game::updateBoard(u8 playerIndex) { //Board gets updated when the playing piece collides with the board
	for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
		for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), states[playerIndex]->rotation)) {
				s8 bX = states[playerIndex]->xOffset + x;
				s8 bY = states[playerIndex]->yOffset + y;
				if (bY >= 0)
					board[bY * game_width + bX] = playerIndex + 1;
				else {
					quit = true;
				}
			}
		}
	}
}
bool Game::isFullRow(int y) {
	for (int x = 0; x < game_width; ++x) { //Every tile in a given row
		if (!board[y * game_width + x])//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}
void Game::clearLines() {
	clearedLines = 0;
	for (int y = 0; y < game_height; ++y) { //Every row
		if (isFullRow(y)) {
			for (int x = 0; x < game_width; ++x) {
				board[y * game_width + x] = 0; //If its a full row, we clear it
			}
			++clearedLines;
			yClearLevel = y;
		}
	}
	lines += clearedLines; //Updating total amount of lines cleared
	while (clearedLines > 0) { //If we clear 1 line, we drop the rest of the board one time. Two times for 2 cleared lines, and so on
		for (int y = yClearLevel; y > 0; --y) {
			for (int x = 0; x < game_width; ++x) {
				board[y * game_width + x] = board[(y - 1) * game_width + x];
			}
		}
		--clearedLines;
	}
}

bool Game::hasCollided(State* state) {
	for (int x = 0; x < state->piece->width; ++x) {
		for (int y = 0; y < state->piece->width; ++y) {
			if (getPiece(x, y, state->piece.get(), state->rotation)) {
				u8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;
				if (bY >= 0 && (board[bY * game_width + bX] >= 1 || bY >= game_height)) {
					return true;
				}
			}
		}
	}
	return false;
}
bool Game::hasLost() {
	for (int x = 0; x < game_width; ++x) {
		if (board[x]) //If there is a piece on the top row, then we lose
			return true;
	}
	return false;
}

bool Game::canWallKick(u8 rotation, u8 playerIndex) {
	for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
		for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), rotation)) {
				s8 bX = states[playerIndex]->xOffset + x;
				s8 bY = states[playerIndex]->yOffset + y;
				if (board[bY * game_width + bX]) {
					return false;
				}
			}
		}
	}
	return true;
}
void Game::wallKick(u8 playerIndex) {
	for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
		for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), states[playerIndex]->rotation)) {
				s8 bX = states[playerIndex]->xOffset + x;
				s8 bY = states[playerIndex]->yOffset + y;
				if (bX < 0) {
					states[playerIndex]->xOffset -= bX;
				}
				if (bX >= game_width) {
					states[playerIndex]->xOffset -= (bX - game_width) + 1;
				}
				if (bY >= game_height) {
					--states[playerIndex]->yOffset;
				}
			}
		}
	}
}

u8 Game::getBottom(u8 playerIndex, bool ghost) {
	u8 checkAmount;
	u8 finalAmount = game_height;
	for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
		for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), states[playerIndex]->rotation)) {
				checkAmount = 0;
				s8 bX = states[playerIndex]->xOffset + x;
				s8 bY = states[playerIndex]->yOffset + y + ghost; //If we are getting the ghost piece, it is 1 value higher
				while (board[bY * game_width + bX] < 1 && bY < game_height) {
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
	renderer.clearRenderer();

	for (u8 playerIndex = 0; playerIndex < states.size(); ++playerIndex) {
		renderPiece(PieceToDraw::NormalPiece, playerIndex);
		u8 ghostPieceDrop = getBottom(playerIndex, true);
		renderPiece(PieceToDraw::GhostPiece, playerIndex, ghostPieceDrop);
	}

	renderBoard();
	renderBorder();
	renderText();

	renderer.showRenderer();
}
//Draws the piece you are playing, the ghost piece for hard-drop accuracy, and the upcoming piece
void Game::renderPiece(PieceToDraw piece, u8 playerIndex, u8 yAmount) {
	for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
		for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), states[playerIndex]->rotation)) {
				switch (piece) {
				case PieceToDraw::NormalPiece:
					renderer.draw(x + states[playerIndex]->xOffset + boardXOffset, y + states[playerIndex]->yOffset + boardYOffset, playerColors[playerIndex].fillColor, sf::Color::White);
					break;
				case PieceToDraw::GhostPiece:
					renderer.draw(x + states[playerIndex]->xOffset + boardXOffset, y + states[playerIndex]->yOffset + boardYOffset + yAmount, playerColors[playerIndex].ghostFillColor, ghostOutlineColor);
					break;
				}
			}
		}
	}
}
void Game::renderBoard() { //Draws all pieces that are currently on the board
	for (int x = 0; x < game_width; ++x) {
		for (int y = 0; y < game_height; ++y) {
			if (board[y * game_width + x])
				renderer.draw(x + boardXOffset, y + boardYOffset, playerColors[board[y * game_width + x] - 1].fillColor, sf::Color::White);
		}
	}
}
void Game::renderBorder() { //Draws the border around the playable area
	for (int x = 7; x < game_width + 9; ++x) {
		renderer.draw(x, 0, sf::Color::White, sf::Color::Blue);
		renderer.draw(x, game_height + 1, sf::Color::White, sf::Color::Blue);
	}
	for (int y = 0; y < game_height + 1; ++y) {
		renderer.draw(7, y, sf::Color::White, sf::Color::Blue);
		renderer.draw(game_width + 8, y, sf::Color::White, sf::Color::Blue);
	}
}
void Game::renderText() {
	std::string lvlStr = "Level: " + std::to_string(level + 1); //Levels are 1-30 but arrays are 0-indexed, so we add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(lines);
	renderer.drawText(width - 200, height / 2, lvlStr);
	renderer.drawText(width - 200, height / 2 + 50, linesStr);
}

bool Game::isValidMove(Move move, u8 playerIndex) { //Can't move into the board or another piece
	for (int x = 0; x < states[playerIndex]->piece->width; ++x) {
		for (int y = 0; y < states[playerIndex]->piece->width; ++y) {
			if (getPiece(x, y, states[playerIndex]->piece.get(), states[playerIndex]->rotation)) {
				u8 bX = states[playerIndex]->xOffset + x;
				u8 bY = states[playerIndex]->yOffset + y;

				if ((bX <= 0 || board[(bY * game_width + bX - 1)]) && move == Move::Left)
					return false;
				if ((bX >= game_width - 1 || board[(bY * game_width + bX + 1)]) && move == Move::Right)
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
					++states[playerOne]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Left) {
				if(isValidMove(Move::Left, playerOne))
					--states[playerOne]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Down) {
				if (isValidMove(Move::Down, playerOne))
					++states[playerOne]->yOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Up) {
				if (canWallKick((states[playerOne]->rotation + 1) % 4, playerOne)) {
					states[playerOne]->rotation = (states[playerOne]->rotation + 1) % 4;
						wallKick(playerOne);
				}
				break;
			}
			if (event.key.code == sf::Keyboard::Space) {
				states[playerOne]->yOffset += getBottom(playerOne);
				break;
			}

			/**
			* Player Two Controls
			*/
			if (event.key.code == sf::Keyboard::D) {
				if (isValidMove(Move::Right, playerTwo))
					++states[playerTwo]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::A) {
				if (isValidMove(Move::Left, playerTwo))
					--states[playerTwo]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::S) {
				if (isValidMove(Move::Down, playerTwo))
					++states[playerTwo]->yOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::W) {
				if (canWallKick((states[playerTwo]->rotation + 1) % 4, playerTwo)) {
					states[playerTwo]->rotation = (states[playerTwo]->rotation + 1) % 4;
					wallKick(playerTwo);
				}
				break;
			}
			if (event.key.code == sf::Keyboard::R) {
				states[playerTwo]->yOffset += getBottom(playerTwo);
				break;
			}

			/**
			* Player Three Controls
			*/
			if (event.key.code == sf::Keyboard::PageDown) {
				if (isValidMove(Move::Right, playerThree))
					++states[playerThree]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Delete) {
				if (isValidMove(Move::Left, playerThree))
					--states[playerThree]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::End) {
				if (isValidMove(Move::Down, playerThree))
					++states[playerThree]->yOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Home) {
				if (canWallKick((states[playerThree]->rotation + 1) % 4, playerThree)) {
					states[playerThree]->rotation = (states[playerThree]->rotation + 1) % 4;
					wallKick(playerThree);
				}
				break;
			}
			if (event.key.code == sf::Keyboard::PageUp) {
				states[playerThree]->yOffset += getBottom(playerThree);
				break;
			}

			/**
			* Player Four Controls
			*/
			if (event.key.code == sf::Keyboard::Numpad6) {
				if (isValidMove(Move::Right, playerFour))
					++states[playerFour]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Numpad4) {
				if (isValidMove(Move::Left, playerFour))
					--states[playerFour]->xOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Numpad5) {
				if (isValidMove(Move::Down, playerFour))
					++states[playerFour]->yOffset;
				break;
			}
			if (event.key.code == sf::Keyboard::Numpad8) {
				if (canWallKick((states[playerFour]->rotation + 1) % 4, playerFour)) {
					states[playerFour]->rotation = (states[playerFour]->rotation + 1) % 4;
					wallKick(playerFour);
				}
				break;
			}
			if (event.key.code == sf::Keyboard::Numpad0) {
				states[playerFour]->yOffset += getBottom(playerFour);
				break;
			}
		}
	}
}