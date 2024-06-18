#pragma once
#include <vector>

#include "Globals.hpp"
#include "Renderer.hpp"

class Board {
public:
	Board(const u8 width, const u8 height);
	void resetBoard();
	u8 getBoardPosition(const s8 x, const u8 y);
	void setBoardPosition(const u8 x, const u8 y, const u8 value);
	void renderBoard(Renderer* const, const std::vector<PlayerColor>&);
private:
	std::vector<u8> m_board;
	const u8 m_boardWidth, m_boardHeight;
};