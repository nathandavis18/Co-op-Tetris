#pragma once
#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;

/**
* This header defines global variables used by multiple/all other classes.
* Also defines the typedefs above.
*/
enum class Move{Left, Right, Down, Rotate, HardDrop, PlayAgain, None};
enum class PieceToDraw{NormalPiece, GhostPiece};

struct PlayerMove {
	Move move;
	u8 player;
};

static bool quit = false;

static constexpr u8 numPlayers = 4;
static constexpr int gameWidth = 10 + ((numPlayers - 1) * 3.4);
static constexpr int gameHeight = 20;
static constexpr int boardHeight = 22;
static constexpr int pieceSize = 30;
static constexpr int sideBuffer = 8; //Gives 8 tiles worth of space on left and right sides
static constexpr int totalWidth = (sideBuffer * 2 + gameWidth) * pieceSize;
static constexpr int totalHeight = (pieceSize * gameHeight) + (pieceSize * 2);
static constexpr int boardXOffset = (totalWidth / pieceSize - gameWidth) / 2; //Offset is left and right sides so we / 2
static constexpr int boardYOffset = (totalHeight / pieceSize - gameHeight) / 2;

static constexpr u8 playerOne = 0;
static constexpr u8 playerTwo = 1;
static constexpr u8 playerThree = 2;
static constexpr u8 playerFour = 3;