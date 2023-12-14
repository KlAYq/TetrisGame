#pragma once
#include "Tetrominoes.h"
// Gameplay board

Board::Board(int numRows, int numCols) : rows(numRows + 4), cols(numCols), grid(numRows + 4, vector<char>(numCols, ' ')) {}
//Extra 4 rows for spawning/checking gameover
void Board::display() const
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (grid[i][j] != ' ')
				drawBlock(colorMap[grid[i][j]], i, j);
			else
			{
				if ((i + j) % 2 != 0 || i < 4)
					drawEmptyCell(colorMap['W'], i, j);
				else
					drawEmptyCell(colorMap['G'], i, j);
			}
		}
	}
}
int Board::isInside(int x, int y){
	if (y < 0)
		return -1; // outer left
	else if (y >= cols)
		return 1; // outer right
	else if (x >= 0 && x < rows)
		return 2; // inside
	else
		return 0; // not inside
}
int Board::getRow(){
	return rows;
}
int Board::getCol(){
	return cols;
}
void Board::clearRow(int x){
	for (int j = 0; j < cols; j++)
		grid[x][j] = ' ';
		
	// move all the above x lines down after clearing line x 	
	for (int i = x; i > 0; i--){
		for (int j = 0; j < cols; j++)
			grid[i][j] = grid[i-1][j];			
	}
}
bool Board::checkClear(int pos, int &lines){
	bool clear = false;
	for (int i = pos; i < rows; i++){
		int isComplete = true;
		for (int j = 0; j < cols; j++)
			if (grid[i][j] == ' '){
				isComplete = false;
			}
		if (isComplete)
		{
			clearRow(i);
			lines++;
			clear = true;
		}
	}
	return clear;
}
char Board::getCell(int x, int y){
	return grid[x][y];
}
bool Board::isValid(){
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < cols; j++)
			if (grid[i][j] != ' ')
				return false;
	return true;
}

void Board::addShape(Tetromino* tetromino){
	pair <int, int> pos = tetromino->getPos();
	vector <pair <int, int> > blocks = tetromino->getBlocks();
	for (int i = 0; i < 4; i++){
		int u = pos.first + blocks[i].first;
		int v = pos.second + blocks[i].second;
		grid[u][v] = tetromino->type();
	}
}


// Tetromino class represents a single Tetris piece
Tetromino::Tetromino(int& x, int& y){
	state = 0;
	pos.first = x; 
	pos.second = y;
}
void Tetromino::move(int offSetX, int offSetY, Board& board){
	for (int i = 0; i < blocks[state].size(); i++){
		int u = pos.first + blocks[state][i].first + offSetX;
		int v = pos.second + blocks[state][i].second + offSetY;
		if (board.isInside(u, v) != 2 || board.getCell(u, v) != ' ')
			return;
	}
	
	pos.first += offSetX;
	pos.second += offSetY;
}
vector <pair<int, int> > Tetromino::getBlocks(){
	return blocks[state];
}
pair<int, int> Tetromino::getPos(){
	return pos;
}
bool Tetromino::collisionCheck(Board& board){
	for (int i = 0; i < 4; i++){
		if (pos.first + blocks[state][i].first + 1  == board.getRow() || board.getCell(pos.first + blocks[state][i].first + 1, pos.second + blocks[state][i].second ) != ' '){
			return true;				
		}
	}
	return false;
}
void Tetromino::rotate(Board& board){
	int tempState = (state + 1) % blocks.size();

	int moveDir = 0;
	for (int i = 0; i < blocks[tempState].size(); i++){
		int u = pos.first + blocks[tempState][i].first;
		int v = pos.second + blocks[tempState][i].second;
		if (board.isInside(u, v) == -1)
			moveDir = max(moveDir, -v);
		else if (board.isInside(u, v) == 1)
			moveDir = min(moveDir, board.getCol() - v - 1);
		if (board.isInside(u, v) == 2 && board.getCell(u, v + moveDir) != ' ')
			return;
	}
	move(0, moveDir, board);

	state++;
	state %= blocks.size();
}
void Tetromino::display(){
	for (int i = 0; i < blocks[state].size(); i++){
		int u = pos.first + blocks[state][i].first;
		int v = pos.second + blocks[state][i].second;
		drawBlock(colorMap[type()], u, v);
	}
}

O_Shape::O_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, 0}, {0, 1}, {1, 0}, {1, 1}});
}
char O_Shape::type()
{
	return 'O';
}
I_Shape::I_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, -1}, {0, 0}, {0, 1}, {0, 2}});
	blocks.push_back({{-1, 1}, {0, 1}, {1, 1}, {2, 1}});
	blocks.push_back({{1, -1}, {1, 0}, {1, 1}, {1, 2}});
	blocks.push_back({{-1, 0}, {0, 0}, {1, 0}, {2, 0}});
}
char I_Shape::type()
{
	return 'I';
}
T_Shape::T_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {1, 0}});
}
char T_Shape::type()
{
	return 'T';
}



S_Shape::S_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {-1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {0, 1}, {1, 1}});
	blocks.push_back({{0, 0}, {0, 1}, {1, -1}, {1, 0}});
	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {1, 0}});
}
char S_Shape::type()
{
	return 'S';
}


Z_Shape::Z_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, 1}, {0, 1}, {1, 0}});
	blocks.push_back({{0, 0}, {0, -1}, {1, 0}, {1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {1, -1}});
}
char Z_Shape::type()
{
	return 'Z';
}


J_Shape::J_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, 1}, {-1, 0}, {1, 0}});
	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, -1}});
}
char J_Shape::type()
{
	return 'J';
}


L_Shape::L_Shape(int x, int y): Tetromino(x, y){
	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {-1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, 1}});
	blocks.push_back({{0, 0}, {1, -1}, {0, -1}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {1, 0}});
}
char L_Shape::type()
{
	return 'L';
}

