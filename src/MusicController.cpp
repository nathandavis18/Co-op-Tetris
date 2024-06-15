#include "../Headers/MusicController.hpp"

MusicController::MusicController() : musicAvailable(true) {
	if (!m_theme.openFromFile("../../../../Music/Tetris-Theme.ogg")) {
		musicAvailable = false;
	}
	else {
		m_theme.setVolume(40);
	}
}

void MusicController::startMusic() {
	if (!musicAvailable) return;

	m_theme.play();
	m_theme.setLoop(true);
}

void MusicController::stopMusic() {
	if (!musicAvailable) return;

	m_theme.stop();
}