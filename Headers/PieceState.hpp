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
		Piece(const std::vector<u8> data, const u8 width) : width(width), data(data) {}

		std::vector<u8> data;
		u8 width;
	};

	/// <summary>
	/// The state of the piece. Which rotation it is in, how far left/right it has moved, and how far down it has moved.
	/// </summary>
	static struct State {
		std::unique_ptr<Piece> piece;
		std::unique_ptr<Piece> nextPiece;
		std::unique_ptr<Piece> heldPiece;

		u8 rotation;
		s8 xOffset;
		u8 yOffset;
		bool canHoldPiece;
	};

public:
	void renderPiece(Renderer* const, const std::unique_ptr<Piece>&, const u8 rotation, const s8 xOffset, const u8 yOffset, PlayerColor* const, const PieceToDraw, const u8 ghostPieceOffset = 0);
	bool getPieceData(const u8 x, const u8 y, const std::unique_ptr<Piece>&, const u8 rotation);

private:
	const sf::Color m_ghostOutlineColor = sf::Color(50, 50, 50, 50);
};

