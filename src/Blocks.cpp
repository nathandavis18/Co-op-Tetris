#include <chrono>

#include "../Headers/Blocks.hpp"

Blocks::Blocks() : m_rng(dev()) {
}

const PieceState::Piece& Blocks::getBlock(){
	std::uniform_int_distribution<std::mt19937::result_type> rand(0, blocks.size() - 1);
	return blocks[rand(m_rng)]; //Returns a block from the list of blocks
}