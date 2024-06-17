#pragma once
#include <vector>
#include <array>

#include "Globals.hpp"
#include "PieceState.hpp"
#include "Blocks.hpp"
#include "Renderer.hpp"
#include "MusicController.hpp"
#include "InputController.hpp"
#include "Board.hpp"

using State = PieceState::State;
using Piece = PieceState::Piece;

/// <summary>
/// The Game Engine class.
/// Controls all events of the game, handing information off to/requesting information from other classes as needed
/// </summary>
class Game{
public:
	Game(u8 numPlayers);
	void loop();

	void newPiece(u8 playerIndex);
	bool getPieceData(u8 x, u8 y, std::unique_ptr<Piece>&, u8 rotation);
	
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
	void renderPiece(PieceToDraw piece, u8 playerIndex, u8 ghostPieceOffset = 0);
	void renderBorder();
	void renderText();

private: //Private function/struct
	void restart();

private: //Private variables
	bool m_quit = false;
	const u8 m_numPlayers;

	Board m_board;

	static constexpr u8 linesToNextLevel = 10;
	u8 m_level = 0;
	u32 m_lines = m_level * linesToNextLevel;
	u8 m_clearedLines = 0;
	u8 m_yClearLevel = 0;

	static constexpr double framesPerSecond = 60.0;
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
	
	std::vector<std::unique_ptr<State>> m_pieceStates;
	Blocks m_blockGenerator;

	std::vector<sf::Time> m_playerTimes;
	sf::Clock m_clock;

	std::vector<PlayerColor> m_playerColors;
    const sf::Color ghostOutlineColor = sf::Color(50, 50, 50, 50);

	static constexpr double m_boardScalingFactor = 3.4; //Board doubles in size for 4 people
	static constexpr u8 m_baseWidth = 10;
	static constexpr u8 m_gameHeight = 20;

	const u16 m_gameWidth, m_totalWidth, m_totalHeight;
	const u16 m_boardXOffset, m_boardYOffset;
};

