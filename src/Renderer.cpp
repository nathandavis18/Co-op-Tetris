#include <filesystem>
#include <iostream>
#include "../Headers/Globals.hpp"
#include "../Headers/Renderer.hpp"

/// <summary>
/// Initializes the window pointer
/// </summary>
/// <param name="window">A pointer to the main window</param>
Renderer::Renderer(const u8 pieceSize, sf::RenderWindow* const window) : m_pieceSize(pieceSize), m_window(window) {}

/// <summary>
/// Clears the renderer
/// </summary>
void Renderer::clearRenderer()
{
	m_window->clear();
}

/// <summary>
/// Displays the renderer
/// </summary>
void Renderer::showRenderer()
{
	m_window->display();
}

void Renderer::drawBorder(const u8 width, const u8 height)
{
	for (s8 x = -1; x <= width; ++x)
	{
		drawPiece(x, -1, sf::Color::White, sf::Color::Blue);
		drawPiece(x, height, sf::Color::White, sf::Color::Blue);
	}
	for (s8 y = -1; y < height; ++y)
	{
		drawPiece(-1, y, sf::Color::White, sf::Color::Blue);
		drawPiece(width, y, sf::Color::White, sf::Color::Blue);
	}
}

/// <summary>
/// Draws the current piece depending on its position
/// </summary>
/// <param name="x">The x position of the piece</param>
/// <param name="y">The y position of the piece</param>
/// <param name="fill">The fill color of the piece</param>
/// <param name="outline">The outline color of the piece</param>
void Renderer::drawPiece(const s8 x, const s8 y, const sf::Color fill, const sf::Color outline)
{
	sf::RectangleShape rect;

	rect.setFillColor(fill);
	rect.setOutlineColor(outline);
	rect.setOutlineThickness(1);

	rect.setSize(sf::Vector2f(m_pieceSize, m_pieceSize));
	rect.setPosition((x * m_pieceSize) + (sideBuffer * m_pieceSize), y * m_pieceSize + (verticalBuffer * m_pieceSize) + m_pieceSize);

	m_window->draw(rect);
}

/// <summary>
/// Draws the text to the board using the specified font
/// </summary>
/// <param name="x">The x position of the text</param>
/// <param name="y">The y position of the text</param>
/// <param name="strToDisplay">The text to display</param>
void Renderer::drawText(const u16 x, const u16 y, const std::string& strToDisplay)
{
	if (!m_font.loadFromFile("./tetris-font.ttf"))
	{
		std::cout << "Error Loading Font" << std::endl;
	}
	else
	{
		m_text.setFont(m_font);
		m_text.setString(strToDisplay);
		m_text.setCharacterSize(24);
		m_text.setFillColor(sf::Color::Cyan);
		m_text.setStyle(sf::Text::Bold);
		m_text.setPosition(x, y);

		m_window->draw(m_text);
	}
}