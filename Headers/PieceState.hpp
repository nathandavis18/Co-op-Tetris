#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
namespace PieceState
{
	/// <summary>
	/// The Piece information, including the piece data and the width of the piece
	/// </summary>
	struct Piece {
		Piece() : width(0), data(0) {}
		Piece(std::vector<u8> data, u8 width) : width(width), data(data) {}
		std::vector<u8> data;
		u8 width;
	};

	/// <summary>
	/// The state of the piece. Which rotation it is in, how far left/right it has moved, and how far down it has moved.
	/// </summary>
	struct State {
		std::unique_ptr<Piece> piece;

		s8 rotation = 0;
		s8 xOffset = 0;
		s8 yOffset = 0;
	};
};

