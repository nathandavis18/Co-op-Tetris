#include <string>
#include <iostream>

#include "../Headers/Game.hpp"


/// <summary>
/// Initialzer for the Game class
/// Initializes the window, renderer, and input controller, and sets the game up.
/// </summary>
Game::Game(const u8 numPlayers, const u8 gameWidth, const u8 gameHeight, const u16 boardXOffset, const u16 boardYOffset, const u16 windowWidth, const u16 windowHeight,
	sf::RenderWindow* const window, Renderer* const renderer, Board* const board, InputController* const inputController,
	MusicController* const musicController, PieceState* const pieceState, Blocks* const blocks) :
	m_numPlayers(numPlayers), m_timeToNextDrop(m_framesPerDrop[m_level] / m_framesPerSecond),
	m_gameWidth(gameWidth), m_gameHeight(gameHeight), m_boardXOffset(boardXOffset), m_boardYOffset(boardYOffset), m_totalWidth(windowWidth), m_totalHeight(windowHeight),
	m_board(board), m_window(window), m_renderer(renderer), m_inputController(inputController), m_musicController(musicController), m_pieceState(pieceState), m_blockGenerator(blocks)
{
	for (u8 playerIndex = 0; playerIndex < numPlayers; ++playerIndex)
	{
		m_playerStates.push_back(std::make_unique<State>(State()));
		m_playerStates[playerIndex]->nextPiece = std::make_unique<Piece>(m_blockGenerator->getBlock());
		newPiece(playerIndex);

		PlayerColor pc;
		if (playerIndex == 0)
		{
			pc.fillColor = sf::Color::Red;
			pc.ghostFillColor = RedPlayerGhostFill;
		}
		else if (playerIndex == 1)
		{
			pc.fillColor = sf::Color::Blue;
			pc.ghostFillColor = BluePlayerGhostFill;
		}
		else if (playerIndex == 2)
		{
			pc.fillColor = sf::Color::Yellow;
			pc.ghostFillColor = YellowPlayerGhostFill;
		}
		else if (playerIndex == 3)
		{
			pc.fillColor = sf::Color::Magenta;
			pc.ghostFillColor = MagentaPlayerGhostFill;
		}
		m_playerColors.push_back(pc);
		m_playerTimes.push_back(sf::milliseconds(m_timeToNextDrop * 1000));
	}
	m_musicController->startMusic();
	updateLevel();
	loop();
}

/// <summary>
/// The main game loop. Calls all necessary functions depending on the state of the game
/// </summary>
void Game::loop()
{
	while (!m_quit && m_window->isOpen())
	{
		input();
		renderGame();
		sf::Time timeDiff = m_clock.restart();
		for (u8 playerIndex = 0; playerIndex < m_numPlayers; ++playerIndex)
		{
			if (m_playerTimes[playerIndex].asMilliseconds() <= 0)
			{
				if (hasCollided(playerIndex))
				{
					updateBoard(playerIndex);

					if (m_numPlayers > 1) movePlayerPieces(playerIndex);

					clearLines();
					m_quit = hasLost();
					updateLevel();
					newPiece(playerIndex);
				}
				else
				{
					++m_playerStates[playerIndex]->yOffset;
				}
				setTimeNextDrop(playerIndex);
			}
			else
			{
				m_playerTimes[playerIndex] -= timeDiff;
			}
		}
	}
	while (m_quit && m_window->isOpen())
	{
		m_renderer->clearRenderer();
		input();
		m_board->renderBoard(m_renderer, m_playerColors);
		m_renderer->drawBorder(m_gameWidth, m_gameHeight);
		renderText();
		m_renderer->showRenderer();
	}
}

/// <summary>
/// Generates a new piece for the player(s) that have had their piece collide with the board
/// </summary>
/// <param name="playerIndex">The current player that needs a new piece</param>
void Game::newPiece(const u8 playerIndex)
{
	std::unique_ptr<State>& state = m_playerStates[playerIndex];

	state->piece = std::move(state->nextPiece);
	state->nextPiece = std::make_unique<Piece>(m_blockGenerator->getBlock());

	state->xOffset = getPlayerStartingXOffset(playerIndex, state->piece->width);
	state->yOffset = 0;
	state->rotation = 0;
	state->canHoldPiece = true;
}

/// <summary>
/// Updates the level based on the amount of lines cleared
/// </summary>
void Game::updateLevel()
{
	m_level = m_lines / 10;
	if (m_level > 29)
	{
		m_level = 29; //Level 29 (30 if not 0 indexing) is the max m_level
	}
}

/// <summary>
/// Sets the time for the current player for the next automatic drop of their piece
/// </summary>
/// <param name="playerIndex">The current player</param>
void Game::setTimeNextDrop(const u8 playerIndex)
{
	m_timeToNextDrop = m_framesPerDrop[m_level] / m_framesPerSecond;
	m_playerTimes[playerIndex] = sf::milliseconds(m_timeToNextDrop * 1000);
}

/// <summary>
/// Adds the dropped piece to the board. Uses the player index in order to maintain piece color
/// </summary>
/// <param name="playerIndex">The current player</param>
void Game::updateBoard(const u8 playerIndex)
{ //Board gets updated when the playing piece collides with the m_board
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	for (u8 x = 0; x < state->piece->width; ++x)
	{
		for (u8 y = 0; y < state->piece->width; ++y)
		{
			if (m_pieceState->getPieceData(x, y, state->piece, state->rotation))
			{
				s8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;

				if (bY >= 0) m_board->setBoardPosition(bX, bY, playerIndex + 1);
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
bool Game::isFullRow(const u8 y)
{
	for (u8 x = 0; x < m_gameWidth; ++x)
	{ //Every tile in a given row
		if (!m_board->getBoardPosition(x, y))//If a single spot in a row isn't a block we don't need to keep checking it
			return false;
	}
	return true;
}

/// <summary>
/// Clears any lines that are full and moves the rows above it down.
/// </summary>
void Game::clearLines()
{
	m_clearedLines = 0;
	for (u8 y = 0; y < m_gameHeight; ++y)
	{ //Every row
		if (isFullRow(y))
		{
			for (u8 x = 0; x < m_gameWidth; ++x)
			{
				m_board->setBoardPosition(x, y, 0); //If its a full row, we clear it
			}
			++m_clearedLines;
			m_yClearLevel = y;
		}
	}
	m_lines += m_clearedLines; //Updating total amount of lines cleared
	while (m_clearedLines > 0)
	{ //If we clear 1 line, we drop the rest of the m_board one time. Two times for 2 cleared lines, and so on
		for (u8 y = m_yClearLevel; y > 0; --y)
		{
			for (u8 x = 0; x < m_gameWidth; ++x)
			{
				m_board->setBoardPosition(x, y, m_board->getBoardPosition(x, (y - 1)));
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
bool Game::hasCollided(const u8 playerIndex)
{
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	for (u8 x = 0; x < state->piece->width; ++x)
	{
		for (u8 y = 0; y < state->piece->width; ++y)
		{
			if (m_pieceState->getPieceData(x, y, state->piece, state->rotation))
			{
				s8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y + 1; //Checking the spot below the piece
				if (bY >= 0 && (m_board->getBoardPosition(bX, bY) || bY >= m_gameHeight))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Game::movePlayerPieces(const u8 currentPlayerIndex)
{
	for (u8 playerIndex = 0; playerIndex < m_numPlayers; ++playerIndex)
	{
		if (playerIndex == currentPlayerIndex) continue;

		std::unique_ptr<State>& state = m_playerStates[playerIndex];

		for (u8 x = 0; x < state->piece->width; ++x)
		{
			for (u8 y = 0; y < state->piece->width; ++y)
			{
				while (m_pieceState->getPieceData(x, y, state->piece, state->rotation) && m_board->getBoardPosition(x + state->xOffset, y + state->yOffset))
				{
					if (state->yOffset - 1 <= 0)
					{
						m_quit = true;
						return;
					}

					--state->yOffset;
				}
			}
		}
	}
}

/// <summary>
/// Checks if the top row contains a piece. If so, the player has lost.
/// This method is called after clearLines in the game loop.
/// </summary>
/// <returns></returns>
bool Game::hasLost()
{
	for (u8 x = 0; x < m_gameWidth; ++x)
	{
		if (m_board->getBoardPosition(x, 0))
		{//If there is a piece on the top row, then we lose
			m_musicController->stopMusic();
			return true;
		}
	}
	return false;
}

bool Game::validRotateStatus(const std::unique_ptr<State>& state, const u8 nextRotation, const s8 xMovement, const s8 yMovement)
{
	for (u8 x = 0; x < state->piece->width; ++x)
	{
		for (u8 y = 0; y < state->piece->width; ++y)
		{
			if (m_pieceState->getPieceData(x, y, state->piece, nextRotation))
			{
				s8 bX = state->xOffset + x + xMovement;
				s8 bY = state->yOffset + y + yMovement;
				if(m_board->getBoardPosition(bX, bY) || bX < 0 || bX >= m_gameWidth || bY >= m_gameHeight) return false;
			}
			
		}
	}
	return true;
}

/// <summary>
/// Run through a set of 5 tests depending on rotation (pulled from tetris wiki https://tetris.wiki/Super_Rotation_System
/// </summary>
/// <param name="playerIndex">The index of the player making the rotation</param>
void Game::tryRotate(const u8 playerIndex)
{
	const std::unique_ptr<State>& state = m_playerStates[playerIndex];
	const u8 nextRotation = (state->rotation + 1) % 4;

	s8 xMovement = 0, yMovement = 0;

	if (state->piece->width == 4)
	{
	    //Test 1 for I Piece
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 2 for I Piece
		if (nextRotation == 1)
		{
			xMovement = -2;
			yMovement = 0;
		}
		else if (nextRotation == 2)
		{
			xMovement = -1;
			yMovement = 0;
		}
		else if (nextRotation == 3)
		{
			xMovement = 2;
			yMovement = 0;
		}
		else
		{
			xMovement = 1;
			yMovement = 0;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 3 for I Piece
		if (nextRotation == 1)
		{
			xMovement = 1;
			yMovement = 0;
		}
		else if (nextRotation == 2)
		{
			xMovement = 2;
			yMovement = 0;
		}
		else if (nextRotation == 3)
		{
			xMovement = -1;
			yMovement = 0;
		}
		else
		{
			xMovement = -2;
			yMovement = 0;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 4 for I Piece:
		if (nextRotation == 1)
		{
			xMovement = -2;
			yMovement = 1;
		}
		else if (nextRotation == 2)
		{
			xMovement = -1;
			yMovement = -2;
		}
		else if (nextRotation == 3)
		{
			xMovement = 2;
			yMovement = -1;
		}
		else
		{
			xMovement = 1;
			yMovement = 2;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 5 for I Piece:
		if (nextRotation == 1)
		{
			xMovement = 1;
			yMovement = -2;
		}
		else if (nextRotation == 2)
		{
			xMovement = 2;
			yMovement = 1;
		}
		else if (nextRotation == 3)
		{
			xMovement = -1;
			yMovement = 2;
		}
		else
		{
			xMovement = -2;
			yMovement = -1;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}
		return; //If we made it here, there are no valid rotations available at current position
	}
	else [[likely]] {
		//Test 1 for non-I Piece
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 2 for non-I Piece
		if (nextRotation == 0 || nextRotation == 1)
		{
			xMovement = -1;
			yMovement = 0;
		}
		else
		{
			xMovement = 1;
			yMovement = 0;
		}

		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}

		//Test 3 for non-I Piece
		if (nextRotation == 1)
		{
			xMovement = -1;
			yMovement = -1;
		}
		else if (nextRotation == 2)
		{
			xMovement = 1;
			yMovement = 1;
		}
		else if (nextRotation == 3)
		{
			xMovement = 1;
			yMovement = -1;
		}
		else
		{
			xMovement = -1;
			yMovement = 1;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}
		
		//Test 4 for non-I Piece
		if (nextRotation == 1 || nextRotation == 3)
		{
			xMovement = 0;
			yMovement = 2;
		}
		else
		{
			xMovement = 0;
			yMovement = -2;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}
		
		//Test 5 for non-I Piece
		if (nextRotation == 1)
		{
			xMovement = -1;
			yMovement = 2;
		}
		else if (nextRotation == 2)
		{
			xMovement = 1;
			yMovement = -2;
		}
		else if (nextRotation == 3)
		{
			xMovement = 1;
			yMovement = 2;
		}
		else
		{
			xMovement = -1;
			yMovement = -2;
		}
		if (validRotateStatus(state, nextRotation, xMovement, yMovement))
		{
			goto rotate;
		}
		return;
	}

	rotate:
		rotatePiece(playerIndex, xMovement, yMovement);
		return;
}

/// <summary>
/// Pushes the piece away from the wall if it can.
/// </summary>
/// <param name="playerIndex">The current player's piece</param>
void Game::rotatePiece(const u8 playerIndex, const u8 x, const u8 y)
{
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	state->rotation = (state->rotation + 1) % 4;
	state->xOffset += x;
	state->yOffset += y;
}

/// <summary>
/// Checks if a given move is valid. Does not allow player to move outside the board, or into another piece on the board.
/// </summary>
/// <param name="move">The move needing to be checked</param>
/// <param name="playerIndex">The player making the move</param>
/// <returns></returns>
bool Game::isValidMove(const Move move, const u8 playerIndex)
{ //Can't move into the m_board or another piece
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	for (u8 x = 0; x < state->piece->width; ++x)
	{
		for (u8 y = 0; y < state->piece->width; ++y)
		{
			if (m_pieceState->getPieceData(x, y, state->piece, state->rotation))
			{
				s8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y;

				if ((bX <= 0 || m_board->getBoardPosition(bX - 1, bY)) && move == Move::Left)
					return false;
				if ((bX >= m_gameWidth - 1 || m_board->getBoardPosition(bX + 1, bY)) && move == Move::Right)
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
void Game::input()
{
	PlayerMove pm = m_inputController->input(m_quit);
	if (pm.player >= m_numPlayers) return;

	switch (pm.move)
	{
	case Move::Right:
		if (isValidMove(Move::Right, pm.player))
			++m_playerStates[pm.player]->xOffset;
		break;

	case Move::Left:
		if (isValidMove(Move::Left, pm.player))
			--m_playerStates[pm.player]->xOffset;
		break;

	case Move::Down:
		if (isValidMove(Move::Down, pm.player))
			m_playerTimes[pm.player] = sf::milliseconds(0);
		break;

	case Move::Rotate:
		tryRotate(pm.player);
		break;

	case Move::HardDrop:
		dropPiece(pm.player);
		break;

	case Move::HoldPiece:
		holdPiece(pm.player);
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
u8 Game::getBottom(const u8 playerIndex)
{
	u8 checkAmount;
	u8 finalAmount = m_gameHeight;
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	for (u8 x = 0; x < state->piece->width; ++x)
	{
		for (u8 y = 0; y < state->piece->width; ++y)
		{
			if (m_pieceState->getPieceData(x, y, state->piece, state->rotation))
			{
				checkAmount = 0;
				s8 bX = state->xOffset + x;
				u8 bY = state->yOffset + y + 1;
				while (!m_board->getBoardPosition(bX, bY) && bY < m_gameHeight)
				{
					++checkAmount;
					++bY;
				}
				if (checkAmount < finalAmount)
				{
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
void Game::dropPiece(const u8 playerIndex)
{
	m_playerStates[playerIndex]->yOffset += getBottom(playerIndex);
	m_playerTimes[playerIndex] = sf::milliseconds(0);
}

void Game::holdPiece(const u8 playerIndex)
{
	std::unique_ptr<State>& state = m_playerStates[playerIndex];
	if (state->canHoldPiece)
	{
		if (state->heldPiece == nullptr)
		{
			state->heldPiece = std::move(state->piece);
			newPiece(playerIndex);
		}
		else
		{
			std::iter_swap(state->piece.get(), state->heldPiece.get());
			state->canHoldPiece = false;
			state->xOffset = getPlayerStartingXOffset(playerIndex, state->piece->width);
			state->rotation = 0;
			state->yOffset = 0;
		}
	}
}

u8 Game::getPlayerStartingXOffset(const u8 playerIndex, const u8 pieceWidth)
{
	return static_cast<u8>((((m_gameWidth * (playerIndex * 2 + 1)) / m_numPlayers) >> 1)) - static_cast<u8>((pieceWidth >> 1));
}

/// <summary>
/// The main function to call all child functions responsible for sending data to the renderer. 
/// </summary>
void Game::renderGame()
{
	m_renderer->clearRenderer();

	for (u8 playerIndex = 0; playerIndex < m_numPlayers; ++playerIndex)
	{
		std::unique_ptr<State>& state = m_playerStates[playerIndex];
		m_pieceState->renderPiece(m_renderer, state->piece, state->rotation, state->xOffset, state->yOffset, &m_playerColors[playerIndex], PieceToDraw::NormalPiece);
		u8 ghostPieceOffset = getBottom(playerIndex);
		m_pieceState->renderPiece(m_renderer, state->piece, state->rotation, state->xOffset, state->yOffset, &m_playerColors[playerIndex], PieceToDraw::GhostPiece, ghostPieceOffset);

		m_pieceState->renderPiece(m_renderer, state->nextPiece, 0, getPlayerStartingXOffset(playerIndex, state->nextPiece->width),
			-verticalBuffer + 1 - (state->nextPiece->width / 4), &m_playerColors[playerIndex], PieceToDraw::NextPiece);

		if (state->heldPiece != nullptr)
		{
			m_pieceState->renderPiece(m_renderer, state->heldPiece, 0, getPlayerStartingXOffset(playerIndex, state->heldPiece->width),
				m_gameHeight + 2, &m_playerColors[playerIndex], PieceToDraw::HeldPiece);
		}

	}
	m_board->renderBoard(m_renderer, m_playerColors);
	m_renderer->drawBorder(m_gameWidth, m_gameHeight);
	renderText();

	m_renderer->showRenderer();
}

/// <summary>
/// Sends the level information and the lines information to the renderer to be displayed
/// </summary>
void Game::renderText()
{
	std::string lvlStr = "Level: " + std::to_string(m_level + 1); //Levels are 1-30 but arrays are 0-indexed, so add 1 purely for display
	std::string linesStr = "Lines: " + std::to_string(m_lines);
	m_renderer->drawText(m_totalWidth - 150, m_totalHeight / 2 - 25, lvlStr);
	m_renderer->drawText(m_totalWidth - 150, m_totalHeight / 2 + 25, linesStr);
	m_renderer->drawText(100, 50, "Next: ");
	m_renderer->drawText(100, m_totalHeight - 100, "Held: ");
}

/// <summary>
/// Resets the game to the starting point. Can only be called after player(s) lost.
/// </summary>
void Game::restart()
{
	m_quit = false;
	m_lines = 0;
	m_clearedLines = 0;
	m_level = 0;
	m_yClearLevel = 0;
	m_timeToNextDrop = m_framesPerDrop[m_level] / m_framesPerSecond;
	for (u8 i = 0; i < m_numPlayers; ++i)
	{
		m_playerStates[i]->piece.reset();
		newPiece(i);
		m_playerTimes[i] = sf::milliseconds(m_timeToNextDrop * 1000);
	}
	m_board->resetBoard();
	m_musicController->startMusic();
	updateLevel();
	loop();
}