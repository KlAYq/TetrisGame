#include "ConsoleDisplay.cpp"
#include "Tetrominoes.cpp"
#include "GameClass.cpp"
#define SCREEN_LENGTH 114
#define SCREEN_WIDTH 48

int main() {
	SetWindowSize(SCREEN_LENGTH, SCREEN_WIDTH);
    SetScreenBufferSize(SCREEN_LENGTH, SCREEN_WIDTH);
    DisableResizeWindow();
    ShowScrollbarOption(0);
    DisableCtrButton(0, 0, 1);
    ShowConsoleCursor(false);
    SetConsoleTitle("Tetrizz");
	srand(time(0)); 
    // Initialize and run the Tetris game
    TetrisGame game(20, 10);
	SetConsoleANSI();
//	game.drawMenu();
	game.displayGame();
	game.displayUI();
    while (true) {
        game.updateGame();
		if (game.gameOver())
			break;
    }

    return 0;
}
