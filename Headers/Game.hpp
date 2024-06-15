#pragma once
#include <array>

#include "Globals.hpp"
#include "PieceState.hpp"
#include "Blocks.hpp"
#include "Renderer.hpp"
#include "MusicController.hpp"
#include "InputController.hpp"

using State = PieceState::State;
using Piece = PieceState::Piece;

/// <summary>
/// The Game Engine class.
/// Controls all events of the game, handing information off to/requesting information from other classes as needed
/// </summary>
class Game{
public:
	Game();
	void newPiece(u8 playerIndex);
	bool getPiece(u8 x, u8 y, std::unique_ptr<Piece>&, u8 rotation);
	
	void loop();
	void updateLevel();
	void setTimeNextDrop(u8 playerIndex);
	void updateBoard(u8 playerIndex);
	bool isFullRow(int y);
	void clearLines();

	bool hasCollided(u8 playerIndex);
	bool hasLost();
	
	bool canWallKick(u8 rotation, u8 playerIndex);
	void wallKick(u8 playerIndex);

	bool isValidMove(Move move, u8 playerIndex);
	void input();
	void dropPiece(u8 playerIndex);

	u8 getBottom(u8 playerIndex);
	void renderGame();
	void renderBoard();
	void renderBorder();
	void renderPiece(PieceToDraw piece, u8 playerIndex, u8 ghostPieceOffset = 0);
	void renderText();

private:
	struct PlayerColor {
		sf::Color fillColor;
		sf::Color ghostFillColor;
	};

	void restart();

private:
	std::array<u16, gameWidth * boardHeight> m_board;
	std::array<PlayerColor, numPlayers> m_playerColors;
	u8 m_level = 0;
	u32 m_lines = m_level * linesToNextLevel;
	u8 m_clearedLines = 0;
	u8 m_yClearLevel = 0;
	std::array<u8, 30> m_framesPerDrop = { //From the tetris wiki
		48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
		5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 1
	};
	double m_timeToNextDrop = m_framesPerDrop[m_level] / framesPerSecond;

	sf::RenderWindow m_window;
	Renderer m_renderer;
	MusicController m_musicController;
	InputController m_inputController;
	

	std::array<std::unique_ptr<State>, numPlayers> m_states;
	Blocks m_blocks;

	std::array<sf::Time, numPlayers> m_times;
	sf::Clock m_clock;

    const sf::Color ghostOutlineColor = sf::Color(50, 50, 50, 50);
	static constexpr u8 linesToNextLevel = 10;
	static constexpr double framesPerSecond = 60.0;
};

