#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
#include "Renderer.hpp"
class PieceState
{
public:
	/// <summary>
	/// The Piece information, including the piece data and the width of the piece
	/// </summary>
	static struct Piece {
		Piece() : width(0), data(0) {}
		Piece(std::vector<u8> data, u8 width) : width(width), data(data) {}
		std::vector<u8> data;
		u8 width;
	};

	/// <summary>
	/// The state of the piece. Which rotation it is in, how far left/right it has moved, and how far down it has moved.
	/// </summary>
	static struct State {
		std::unique_ptr<Piece> piece;

		s8 rotation = 0;
		s8 xOffset = 0;
		s8 yOffset = 0;
	};

public:
	void renderPiece(Renderer*, std::unique_ptr<State>&, PlayerColor*, PieceToDraw, u16 boardXOffset, u16 boardYOffset, u8 ghostPieceOffset = 0);
	bool getPieceData(u8 x, u8 y, std::unique_ptr<Piece>&, u8 rotation);

private:
	const sf::Color m_ghostOutlineColor = sf::Color(50, 50, 50, 50);
};

