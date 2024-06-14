#include <filesystem>
#include <iostream>
#include "../Headers/Globals.hpp"
#include "../Headers/Renderer.hpp"

Renderer::Renderer(sf::RenderWindow* window) : m_window(window) { }

void Renderer::clearRenderer() {
	m_window->clear();
}

void Renderer::showRenderer() {
	m_window->display();
}

void Renderer::draw(const int x, const int y, const sf::Color fill, const sf::Color outline) {
	sf::RectangleShape rect;

	rect.setFillColor(fill);
	rect.setOutlineColor(outline);
	rect.setOutlineThickness(1);
	
	rect.setSize(sf::Vector2f(pieceSize, pieceSize));
	rect.setPosition(x * pieceSize, y * pieceSize);
	
	m_window->draw(rect);
}

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