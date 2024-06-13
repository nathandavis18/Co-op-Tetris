#include "../Headers/Game.hpp"

int main() {
	static_assert(numPlayers > 0 && numPlayers <= 4, "numPlayers must be between 1-4.");
	Game game;
	return 0;
}