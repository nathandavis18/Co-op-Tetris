#pragma once
#include <memory>
#include <vector>
#include "Globals.hpp"
namespace PieceState
{
	struct Piece {
		Piece() : width(0), data(0) {}
		Piece(std::vector<u8> data, u8 width) : width(width), data(data) {}
		std::vector<u8> data;
		u8 width;
	};

	struct State {
		std::unique_ptr<Piece> piece;

		s8 rotation = 0;
		s8 xOffset = 0;
		s8 yOffset = 0;
	};
};

