#include <filesystem>
#include <iostream>
#include "../Headers/Globals.hpp"
#include "../Headers/Renderer.hpp"

/// <summary>
/// Initializes the window pointer
/// </summary>
/// <param name="window">A pointer to the main window</param>
Renderer::Renderer(sf::RenderWindow* window) : m_window(window) { }

/// <summary>
/// Clears the renderer
/// </summary>
void Renderer::clearRenderer() {
	m_window->clear();
}

/// <summary>
/// Displays the renderer
/// </summary>
void Renderer::showRenderer() {
	m_window->display();
}

/// <summary>
/// Draws the current piece depending on its position
/// </summary>
/// <param name="x">The x position of the piece</param>
/// <param name="y">The y position of the piece</param>
/// <param name="fill">The fill color of the piece</param>
/// <param name="outline">The outline color of the piece</param>
void Renderer::draw(const int x, const int y, const sf::Color fill, const sf::Color outline) {
	sf::RectangleShape rect;

	rect.setFillColor(fill);
	rect.setOutlineColor(outline);
	rect.setOutlineThickness(1);
	
	rect.setSize(sf::Vector2f(pieceSize, pieceSize));
	rect.setPosition(x * pieceSize, y * pieceSize);
	
	m_window->draw(rect);
}

/// <summary>
/// Draws the text to the board using the specified font
/// </summary>
/// <param name="x">The x position of the text</param>
/// <param name="y">The y position of the text</param>
/// <param name="strToDisplay">The text to display</param>
void Renderer::drawText(const int x, const int y, const std::string& strToDisplay) {
	if (!m_font.loadFromFile("../../../../Font/tetris-font.ttf")) {
		std::cout << "Error Loading Font" << std::endl;
	}
	else {
		m_text.setFont(m_font);
		m_text.setString(strToDisplay);
		m_text.setCharacterSize(24);
		m_text.setFillColor(sf::Color::Cyan);
		m_text.setStyle(sf::Text::Bold);
		m_text.setPosition(x, y);
		
		m_window->draw(m_text);
	}
}