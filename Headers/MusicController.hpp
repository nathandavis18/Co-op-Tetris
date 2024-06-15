#pragma once
#include <SFML/Audio.hpp>
class MusicController {
public:
	MusicController();
	void startMusic();
	void stopMusic();
private:
	sf::Music m_theme;
	bool musicAvailable;
};