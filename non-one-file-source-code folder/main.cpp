#include "ConsoleDisplay.cpp"
#include "Tetrominoes.cpp"
#include "GameClass.cpp"


int main() {
	srand(time(0)); 
    // Initialize and run the Tetris game
    TetrisGame game(20, 10);
	SetConsoleANSI();
	game.displayGame();
	game.displayUI();
    while (true) {
        game.updateGame();
		if (game.gameOver())
			break;
    }

    return 0;
}
