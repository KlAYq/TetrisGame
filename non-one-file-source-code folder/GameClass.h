#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
//#include <utility>
#include <conio.h>
#include <windows.h>
#include <map>

using namespace std;


class TetrisGame {
private:
    Board board;
    Tetromino* currentTetromino;
	const time_t starttime; //Start time
	time_t nowtime; //Current time, only update once per second
	int clearedlines;  //Number of cleared lines in current level
	int score; 
	int level;	//A level requires 2*level + 1 lines to be cleared

public:
    TetrisGame(int numRows, int numCols);

    void spawnTetromino(int x, int y);
	void update_score(int newclearedlines);
	bool gameOver();
    void updateGame();
    void displayGame() const;
	void displayUI() const;
};

