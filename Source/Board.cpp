#include "Board.h"
extern map <char, string> colorMap;

// Gameplay board
Board::Board(int numRows, int numCols) : 
	    rows(numRows + 4), cols(numCols), 
		grid(numRows + 4, vector<char>(numCols, ' ')) {}

//Extra 4 rows for spawning/checking gameover
void Board::display() const
{
	for (int i = 4; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (grid[i][j] != ' ')
				drawBlock(colorMap[grid[i][j]], i + BORDER_W, j + BORDER_L);
			else
			{
				if ((i + j) % 2 != 0 || i < 4)
					drawBlock(colorMap['W'], i + BORDER_W, j + BORDER_L);
				else
					drawBlock(colorMap['G'], i + BORDER_W, j + BORDER_L);
			}
		}
	}
}

int Board::isInside(int x, int y)
{
	if (y < 0) return -1; 					// outer left
	else if (y >= cols) return 1; 			// outer right
	else if (x >= 0 && x < rows) return 2; 	// inside
	else return 0; 							// not inside
}

int Board::getRow() { return rows; }

int Board::getCol() { return cols; }

void Board::clearRow(int x)
{
	for (int j = 0; j < cols; j++)
		grid[x][j] = ' ';
		
	// move all the above x lines down after clearing line x 	
	for (int i = x; i > 0; i--)
	{
		for (int j = 0; j < cols; j++)
			grid[i][j] = grid[i-1][j];			
	}
}
bool Board::checkClear(int pos, int &lines)
{
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

char Board::getCell(int x, int y) { return grid[x][y]; }

bool Board::isValid()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < cols; j++)
			if (grid[i][j] != ' ')
				return false;
	return true;
}

void Board::addShape(Tetromino* tetromino)
{
	pair <int, int> pos = tetromino->getPos();
	vector <pair <int, int> > blocks = tetromino->getBlocks();
	for (int i = 0; i < 4; i++){
		int u = pos.first + blocks[i].first;
		int v = pos.second + blocks[i].second;
		grid[u][v] = tetromino->type();
	}
}

void Board::ResetBoard(){
	grid = vector<vector<char>>(rows + 4, vector<char>(cols, ' '));
}

