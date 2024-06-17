#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#include "Globals.hpp"

/// <summary>
/// This class abstracts the rendering information away from the Game class
/// </summary>
class Renderer{
public:
	const Renderer(const u8 pieceSize, sf::RenderWindow* window);
	void clearRenderer();
	void showRenderer();
	void drawBorder(u8 gameWidth, u8 gameHeight);
	void drawPiece(const int x, const int y, const sf::Color fill, const sf::Color outline);
	void drawText(const int x, const int y, const std::string& strToDisplay);
private:
	sf::Font m_font;
	sf::Text m_text;
	sf::RenderWindow* m_window;
	const u8 m_pieceSize;
};

