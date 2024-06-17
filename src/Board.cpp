#include "../Headers/Board.hpp"

Board::Board(u8 width, u8 height) : m_boardWidth(width), m_boardHeight(height) {
	m_board.resize(m_boardHeight * m_boardWidth);
	resetBoard();
}

void Board::resetBoard() {
	std::fill(m_board.begin(), m_board.end(), 0);
}

void Board::setBoardPosition(u8 x, u8 y, u8 value) {
	m_board[y * m_boardWidth + x] = value;
}

u8 Board::getBoardPosition(u8 x, u8 y) {
	return m_board[y * m_boardWidth + x];
}

void Board::renderBoard(Renderer* renderer, std::vector<PlayerColor>& playerColors, u16 boardXOffset, u16 boardYOffset) {
	for (int x = 0; x < m_boardWidth; ++x) {
		for (int y = 0; y < m_boardHeight; ++y) {
			if (getBoardPosition(x, y)) {
				PlayerColor playerColor = playerColors[getBoardPosition(x, y) - 1];
				renderer->draw(x + boardXOffset, y + boardYOffset, playerColor.fillColor, sf::Color::White);
			}
		}
	}
}