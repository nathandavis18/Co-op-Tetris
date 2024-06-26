#include <chrono>

#include "../Headers/Blocks.hpp"

/// <summary>
/// Initializes the rng generator with the random device
/// </summary>
Blocks::Blocks() : m_rng(m_dev()) {}

/// <summary>
/// Generates a random number between 0 and m_blocks.size() - 1 inclusive, and returns the block at that given index.
/// </summary>
/// <returns></returns>
const PieceState::Piece& Blocks::getBlock()
{
	std::uniform_int_distribution<std::mt19937::result_type> rand(0, m_blocks.size() - 1);
	return m_blocks[rand(m_rng)]; //Returns a block from the list of blocks
}