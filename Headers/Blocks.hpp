#pragma once
#include <vector>
#include <random>

#include "Globals.hpp"
#include "PieceState.hpp"

class Blocks {
public:
	Blocks();
	const PieceState::Piece& getBlock();
private:
	const std::vector<u8> oBlock = {
		1, 1,
		1, 1
	};
	const std::vector<u8> sBlock = {
		0, 1, 1,
		1, 1, 0,
		0, 0, 0
	};
	const std::vector<u8> zBlock = {
		1, 1, 0,
		0, 1, 1,
		0, 0, 0
	};
	const std::vector<u8> lBlock = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 1
	};
	const std::vector<u8> jBlock = {
		0, 1, 0,
		0, 1, 0,
		1, 1, 0
	};
	const std::vector<u8> tBlock = {
		0, 1, 0,
		1, 1, 1,
		0, 0, 0
	};
	const std::vector<u8> iBlock = {
		0, 0, 1, 0,
		0, 0, 1, 0,
		0, 0, 1, 0,
		0, 0, 1, 0
	};
	const std::vector<PieceState::Piece> blocks = {
		PieceState::Piece(oBlock, 2),
		PieceState::Piece(sBlock, 3),
		PieceState::Piece(zBlock, 3),
		PieceState::Piece(lBlock, 3),
		PieceState::Piece(jBlock, 3),
		PieceState::Piece(tBlock, 3),
		PieceState::Piece(iBlock, 4)
	};

	std::random_device dev;
	std::mt19937 m_rng;
};