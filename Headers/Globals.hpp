#pragma once
#include <cstdint>

#include<SFML/Graphics/Color.hpp>

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using s8 = std::int8_t;

/**
* This header defines global variables used by multiple/all other classes.
* Also defines the typedefs above.
*/
enum Move { Right = 0, Left = 1, Down = 2, Rotate = 3, HardDrop = 4, HoldPiece = 5, PlayAgain = 6, None };
enum class PieceToDraw { NormalPiece, GhostPiece, HeldPiece, NextPiece};


struct PlayerMove {
	Move move;
	u8 player;
};

static const sf::Color RedPlayerGhostFill = sf::Color(150, 0, 0, 100);
static const sf::Color BluePlayerGhostFill = sf::Color(0, 0, 150, 100);
static const sf::Color YellowPlayerGhostFill = sf::Color(150, 150, 0, 100);
static const sf::Color MagentaPlayerGhostFill = sf::Color(100, 0, 50, 100);

struct PlayerColor {
	sf::Color fillColor;
	sf::Color ghostFillColor;
};

static constexpr u8 sideBuffer = 8;
static constexpr s8 verticalBuffer = 6;