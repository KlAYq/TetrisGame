#pragma once
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include "ConsoleDisplay.h"

#define BORDER_W -2 //
#define BORDER_L 14 //6

using namespace std;

class Tetromino;

// Gameplay board
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


// Tetromino class represents a single Tetris piece
class Tetromino 
{
protected:
	int state;
    pair <int, int> pos;
    vector <vector <pair <int, int> > > blocks;
public:
	Tetromino(int x = 0, int y = 0);
	void setPos(int x, int y);
    void move(int offSetX, int offSetY, Board& board);
	vector <pair<int, int> > getBlocks();
	pair<int, int> getPos();
	bool collisionCheck(Board& board);
	void rotate(Board& board);
    void display();
    void clear();
	virtual char type() = 0;
};

// Teromino types
class O_Shape : public Tetromino {
public:
    O_Shape(int x = 0, int y = 0);
	char type();
};

class I_Shape : public Tetromino {
public:
    I_Shape(int x = 0, int y = 0);
    char type();
};

class T_Shape : public Tetromino {
public: 
    T_Shape(int x = 0, int y = 0);
	char type();
};

class S_Shape : public Tetromino {
public:
    S_Shape(int x = 0, int y = 0);
	char type();
};

class Z_Shape : public Tetromino {
public:
    Z_Shape(int x = 0, int y = 0);
	char type();
};

class J_Shape : public Tetromino {
public:
    J_Shape(int x = 0, int y = 0);
	char type();
};

class L_Shape : public Tetromino {
public:
    L_Shape(int x = 0, int y = 0);
	char type();
};


