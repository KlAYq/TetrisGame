#include "ConsoleDisplay.cpp"
#include "Tetrominoes.cpp"
#include "GameClass.cpp"
#define LENGTH 1000
#define WIDTH 1000

int main() {
	SetWindowSize(LENGTH, WIDTH);
    SetScreenBufferSize(LENGTH, WIDTH);
    DisableResizeWindow();
    ShowScrollbarOption(0);
    DisableCtrButton(0, 0, 1);
    SetConsoleTitle("Tetrizz");
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
