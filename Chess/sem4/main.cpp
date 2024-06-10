#include "Game.h"
#include <locale>

int main() {

	// Ustawienie lokalizacji na polsk¹
	setlocale(LC_CTYPE, "Polish");

	Game game;
	game.GameLoop();


	return 0;
}