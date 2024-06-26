#pragma once
#include <SFML/Audio.hpp>

/// <summary>
/// This class abstracts the sf::Music away from the Game class
/// Will add sf::Sound to this class as well in the future for drop/line-cleared sounds.
/// </summary>
class MusicController
{
public:
	MusicController();
	void startMusic();
	void stopMusic();
private:
	sf::Music m_theme;
	bool musicAvailable;
};