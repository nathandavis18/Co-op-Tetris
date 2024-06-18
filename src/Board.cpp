#include "../Headers/Board.hpp"

Board::Board(const u8 width, const u8 height) : m_boardWidth(width), m_boardHeight(height) {
	m_board.resize(width * height);
	resetBoard();
}

void Board::resetBoard() {
	std::fill(m_board.begin(), m_board.end(), 0);
}

void Board::setBoardPosition(const u8 x, const u8 y, const u8 value) {
	m_board[y * m_boardWidth + x] = value;
}

u8 Board::getBoardPosition(const s8 x, const u8 y) {
	return m_board[y * m_boardWidth + x];
}

void Board::renderBoard(Renderer* const renderer, const std::vector<PlayerColor>& playerColors) {
	for (int x = 0; x < m_boardWidth; ++x) {
		for (int y = 0; y < m_boardHeight; ++y) {
			if (getBoardPosition(x, y)) {
				PlayerColor playerColor = playerColors[getBoardPosition(x, y) - 1];
				renderer->drawPiece(x, y, playerColor.fillColor, sf::Color::White);
			}
		}
	}
}