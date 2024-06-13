#pragma once
#include <string>
#include <SFML/Graphics.hpp>

class Renderer
{
public:
	const Renderer(sf::RenderWindow* window);
	void clearRenderer();
	void showRenderer();
	void draw(const int x, const int y, const sf::Color fill, const sf::Color outline);
	void drawText(const int x, const int y, const std::string strToDisplay);
private:
	sf::Font font;
	sf::Text text;
	sf::RenderWindow* m_window;
};

