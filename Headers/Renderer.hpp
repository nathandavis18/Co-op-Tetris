#pragma once
#include <string>
#include <SFML/Graphics.hpp>

/// <summary>
/// This class abstracts the rendering information away from the Game class
/// </summary>
class Renderer{
public:
	const Renderer(sf::RenderWindow* window);
	void clearRenderer();
	void showRenderer();
	void draw(const int x, const int y, const sf::Color fill, const sf::Color outline);
	void drawText(const int x, const int y, const std::string& strToDisplay);
private:
	sf::Font m_font;
	sf::Text m_text;
	sf::RenderWindow* m_window;
};

