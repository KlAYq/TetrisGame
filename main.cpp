#include "ConsoleDisplay.h"
#include "Tetrominoes.h"
#include "GameClass.h"
#define SCREEN_LENGTH 114
#define SCREEN_WIDTH 48

int main() 
{
    system("cls");
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
//	game.displayUI();
	game.GameInit();
//	game.drawCredits();
    
    return 0;
}
