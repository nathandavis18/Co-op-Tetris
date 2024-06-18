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
	Game(const u8 numPlayers, const u8 gameWidth, const u8 gameHeight, const u16 boardXOffset, const u16 boardYOffset, const u16 windowWidth, const u16 windowHeight, 
			sf::RenderWindow* const, Renderer* const, Board* const, InputController* const, MusicController* const, PieceState* const, Blocks* const);
private: //Private functions - Only the game class should be calling these
	void loop();

	void newPiece(const u8 playerIndex);

	void updateLevel();
	void setTimeNextDrop(const u8 playerIndex);
	void updateBoard(const u8 playerIndex);
	bool isFullRow(const u8 y);
	void clearLines();

	bool hasCollided(const u8 playerIndex);
	bool hasLost();

	bool canWallKick(const u8 rotation, const u8 playerIndex);
	void wallKick(const u8 playerIndex);

	bool isValidMove(const Move move, const u8 playerIndex);
	void input();
	void dropPiece(const u8 playerIndex);
	void holdPiece(const u8 playerIndex);
	u8 getBottom(const u8 playerIndex);

	void renderGame();
	void renderText();
	void restart();

private: //Private variables
	bool m_quit = false;
	const u8 m_numPlayers;

	static constexpr u8 linesToNextLevel = 10;
	u8 m_level = 0;
	u32 m_lines = m_level * linesToNextLevel;
	u8 m_clearedLines = 0;
	u8 m_yClearLevel = 0;

	static constexpr double m_framesPerSecond = 60.0;
	const std::array<u8, 30> m_framesPerDrop = { //From the tetris wiki
		48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
		5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 1
	};
	double m_timeToNextDrop;

	sf::RenderWindow* const m_window;
	Renderer* const m_renderer;
	MusicController* const m_musicController;
	InputController* const m_inputController;
	Board* const m_board;
	PieceState* const m_pieceState;
	Blocks* const m_blockGenerator;
	
	std::vector<std::unique_ptr<State>> m_playerStates;

	std::vector<sf::Time> m_playerTimes;
	sf::Clock m_clock;

	std::vector<PlayerColor> m_playerColors;

	const u16 m_gameWidth, m_gameHeight, m_totalWidth, m_totalHeight;
	const u16 m_boardXOffset, m_boardYOffset;
};

