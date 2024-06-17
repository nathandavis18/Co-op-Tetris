#pragma once
#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using s8 = std::int8_t;

/**
* This header defines global variables used by multiple/all other classes.
* Also defines the typedefs above.
*/
enum Move { Right = 0, Left = 1, Down = 2, Rotate = 3, HardDrop = 4, PlayAgain = 5, None };
enum class PieceToDraw { NormalPiece, GhostPiece };

struct PlayerMove {
	Move move;
	u8 player;
};

static constexpr u8 pieceSize = 30;
static constexpr u8 sideBuffer = 8; //Gives 8 tiles worth of space on left and right sides

static constexpr u8 playerOne = 0;
static constexpr u8 playerTwo = 1;
static constexpr u8 playerThree = 2;
static constexpr u8 playerFour = 3;