#include "Game.h"
#include <locale>

int main() {

	// Ustawienie lokalizacji na polsk�
	setlocale(LC_CTYPE, "Polish");

	Game game;
	game.GameLoop();


	return 0;
}