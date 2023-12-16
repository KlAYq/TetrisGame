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

#pragma once
#include "Board.h"
using namespace std;

class Board;
// Tetromino class represents a single Tetris piece
class Tetromino 
{
protected:
	int state; // represents the rotation state: state 0/1/2/3 <=> 0/90/180/270 degree
    pair <int, int> pos; // position of the Tetromino in the grid
    vector <vector <pair <int, int> > > blocks; // position of all the block of the Tetromino relative to pos
public:
	Tetromino(int x = 0, int y = 0);
	virtual char type() = 0;

	pair<int, int> getPos();
	vector <pair<int, int> > getBlocks();
	void setPos(int x, int y);
    
	void move(int offSetX, int offSetY, Board& board);
	bool collisionCheck(Board& board);
	void rotate(Board& board);

    void display();
    void clear();	
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


