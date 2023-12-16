#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <map>
#include <fstream>
#include <string>
#include <mmsystem.h>
#include "Tetrominoes.h"
#include "Board.h"
using namespace std;

class TetrisGame
{
private:
    Board board;
    Tetromino* currentTetromino;
	vector <Tetromino*> tetrominoQueue;
	time_t starttime; 						//Start time
	time_t nowtime; 						//Current time, only update once per second
	int clearedlines;  						//Number of cleared lines in current level
	int level;								//A level requires 2*level + 1 lines to be cleared
	int score;
public:
    TetrisGame(int numRows, int numCols);
    ~TetrisGame();
	void ResetGame();
	
    void spawnTetromino(int x, int y);
	void update_score(int newclearedlines);
	bool gameOver();
    void updateGame();
    void displayTetrominoQueue() const;

    void displayGame() const;
	void displayUI() const;
	int drawMenu() const;
	void drawHowToPlay() const;
	void drawGameOver() const;
	void drawCredits() const;
	
	void GameInit();
};
