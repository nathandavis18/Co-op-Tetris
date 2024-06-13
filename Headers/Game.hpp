#pragma once
#include <array>

#include "Globals.hpp"
#include "PieceState.hpp"
#include "Blocks.hpp"
#include "Renderer.hpp"

using State = PieceState::State;
using Piece = PieceState::Piece;

class Game{
	sf::Event event{};
public:
	Game();
	void newPiece(u8 playerIndex);
	bool getPiece(u8 x, u8 y, Piece*, u8 rotation);
	
	void loop();
	void getLevel();
	void setTimeNextDrop();
	void updateBoard(u8 playerIndex);
	bool isFullRow(int y);
	void clearLines();

	bool hasCollided(State* state);
	bool hasLost();
	
	bool canWallKick(u8 rotation, u8 playerIndex);
	void wallKick(u8 playerIndex);

	bool isValidMove(Move move, u8 playerIndex);
	void input();

	u8 getBottom(u8 playerIndex, bool = false);
	void renderGame();
	void renderBoard();
	void renderBorder();
	void renderPiece(PieceToDraw piece, u8 playerIndex, u8 yAmount = 0);
	void renderText();

private:
	struct PlayerColor {
		sf::Color fillColor;
		sf::Color ghostFillColor;
	};

private:
	std::array<u16, game_width * board_height> board;
	std::array<PlayerColor, numPlayers> playerColors;
	u8 level = 0;
	u32 lines = level * linesToNextLevel;
	u8 clearedLines = 0;
	u8 yClearLevel = 0;
	std::array<u8, 30> framesPerDrop = { //From the tetris wiki
		48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
		5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2,
		2, 2, 2, 2, 2, 2, 1
	};
	double timeToNextDrop = framesPerDrop[level] / framesPerSecond;

	sf::RenderWindow m_window;
	Renderer renderer;

	std::array<std::unique_ptr<State>, numPlayers> states;
	Blocks blocks;

	sf::Time time = sf::milliseconds(1000);
	sf::Clock clock;
    const sf::Color ghostOutlineColor = sf::Color(50, 50, 50, 50);

	static constexpr u8 linesToNextLevel = 10;
	static constexpr double framesPerSecond = 60.0;
	static constexpr u8 playerOne = 0;
	static constexpr u8 playerTwo = 1;
	static constexpr u8 playerThree = 2;
	static constexpr u8 playerFour = 3;
};

