#pragma once
#include <vector>

#include "Globals.hpp"
#include "Renderer.hpp"

struct PlayerColor {
	sf::Color fillColor;
	sf::Color ghostFillColor;
};

class Board {
public:
	Board(u8 width, u8 height);
	void resetBoard();
	u8 getBoardPosition(u8 x, u8 y);
	void setBoardPosition(u8 x, u8 y, u8 value);
	void renderBoard(Renderer* renderer, std::vector<PlayerColor>& playerColors, u16 boardXOffset, u16 boardYOffset);
private:
	std::vector<u8> m_board;
	const u8 m_boardWidth, m_boardHeight;
};