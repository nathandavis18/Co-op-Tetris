#include "../Headers/MusicController.hpp"

/// <summary>
/// Initalizes the theme song file. If it can't be opened, don't allow the music to be played.
/// Also turns down the volume so it isn't too loud.
/// </summary>
MusicController::MusicController() : musicAvailable(true) {
	if (!m_theme.openFromFile("./Tetris-Theme.ogg")) {
		musicAvailable = false;
	}
	else {
		m_theme.setVolume(40);
	}
}

/// <summary>
/// Starts the music and sets the loop.
/// </summary>
void MusicController::startMusic() {
	if (!musicAvailable) return;

	m_theme.play();
	m_theme.setLoop(true);
}

/// <summary>
/// Stops the music after losing.
/// </summary>
void MusicController::stopMusic() {
	if (!musicAvailable) return;

	m_theme.stop();
}