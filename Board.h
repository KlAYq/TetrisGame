#pragma once
#include "Tetrominoes.h"
// Gameplay board
class Tetromino;

class Board
{
private:
    const int rows;
    const int cols;
    vector<vector<char>> grid;
public:
    Board(int numRows, int numCols);

	int getRow();
	int getCol();
	
	int isInside(int x, int y);
	bool checkClear(int pos, int &lines);
	char getCell(int x, int y);
	bool isValid();
	void addShape(Tetromino* tetromino);
	
	void ResetBoard();
	void display() const;
	void clearRow(int x);
	void clearSFX(int x);
};


