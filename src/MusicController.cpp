#include "../Headers/MusicController.hpp"

#include <iostream>

MusicController::MusicController() {
	if (!m_theme.openFromFile("../../../../Music/Tetris-Theme.ogg")) {
		std::cout << "error opening music";
	}
}

void MusicController::startMusic() {
	m_theme.play();
	m_theme.setLoop(true);
}

void MusicController::stopMusic() {
	m_theme.stop();
}