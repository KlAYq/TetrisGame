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

	void display() const;
	void ResetBoard();
	int isInside(int x, int y);
	int getRow();
	int getCol();
	void clearRow(int x);
	bool checkClear(int pos, int &lines);
	char getCell(int x, int y);
	bool isValid();
	void addShape(Tetromino* tetromino);
};


