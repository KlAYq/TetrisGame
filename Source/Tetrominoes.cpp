#include "Tetrominoes.h"

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

// Tetromino class represents a single Tetris piece
Tetromino::Tetromino(int x, int y)
{
	state = 0;
	pos.first = x; 
	pos.second = y;
}

void Tetromino::setPos(int x, int y) 
{
	pos.first = x;
	pos.second = y;
}

void Tetromino::clear(){
	for (int i = 0; i < blocks[state].size(); i++){
		int u = pos.first + blocks[state][i].first;
		int v = pos.second + blocks[state][i].second;
		if (u < 4)
			continue;
		else 
		{
			if ((u + v) % 2 == 0)
				drawBlock(colorMap['G'], u + BORDER_W, v + BORDER_L);
			else
				drawBlock(colorMap['W'], u + BORDER_W, v + BORDER_L);
		}
	}
}

void Tetromino::move(int offSetX, int offSetY, Board& board)
{
	for (int i = 0; i < blocks[state].size(); i++)
	{
		int u = pos.first + blocks[state][i].first + offSetX;
		int v = pos.second + blocks[state][i].second + offSetY;
		if (board.isInside(u, v) != 2 || board.getCell(u, v) != ' ')
			return;
	}
	clear();
	pos.first += offSetX;
	pos.second += offSetY;
	display();
}

vector <pair<int, int> > Tetromino::getBlocks() { return blocks[state]; }

pair<int, int> Tetromino::getPos() { return pos; }

bool Tetromino::collisionCheck(Board& board)
{
	for (int i = 0; i < 4; i++)
	{
		if (pos.first + blocks[state][i].first + 1  == board.getRow() || board.getCell(pos.first + blocks[state][i].first + 1, pos.second + blocks[state][i].second ) != ' '){
			return true;				
		}
	}
	return false;
}

void Tetromino::rotate(Board& board){
	int tempState = (state + 1) % blocks.size();

	int I_shapeOffset = 0;
		if (type() == 'I' && (tempState == 1 || tempState == 2))
			I_shapeOffset = 1;
	pair <int, int> moveDir = {0, 0};
	
	for (int i = 0; i < blocks[tempState].size(); i++)
	{
		int u = pos.first + blocks[tempState][i].first;
		int v = pos.second + blocks[tempState][i].second;
		if ((board.isInside(u + moveDir.first, v + moveDir.second) != 2) || (board.isInside(u + moveDir.first, v + moveDir.second) == 2 && board.getCell(u + moveDir.first, v + moveDir.second) != ' ')){ // one of the block hit the other block -> move up/left/right accordingly
				if (pos.second + I_shapeOffset > v) 		// need moving to the right
						moveDir.second++;
				else if (pos.second + I_shapeOffset < v) 	// need moving to the left
						moveDir.second--;
				else 										// move upward
					moveDir.first--;
			}
	}
	for (int i = 0; i < blocks[tempState].size(); i++)
	{
		int u = pos.first + blocks[tempState][i].first + moveDir.first;
		int v = pos.second + blocks[tempState][i].second + moveDir.second;
	if ((board.isInside(u, v) != 2) ||(board.isInside(u, v) == 2 && board.getCell(u, v) != ' ')) 
		return;
	}
	move(moveDir.first, moveDir.second, board);
	clear();
	state++;
	state %= blocks.size();
	display();
}

void Tetromino::display()
{
	for (int i = 0; i < blocks[state].size(); i++)
	{
		int u = pos.first + blocks[state][i].first;
		int v = pos.second + blocks[state][i].second;
		if (u < 4)
			continue;
		else 
			drawBlock(colorMap[type()], u + BORDER_W, v + BORDER_L);
	}
}

O_Shape::O_Shape(int x, int y): Tetromino(x, y)
{	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {-1, -1}}); }

char O_Shape::type() { return 'O'; }

I_Shape::I_Shape(int x, int y): Tetromino(x, y)
{
	blocks.push_back({{0, -1}, {0, 0}, {0, 1}, {0, 2}});
	blocks.push_back({{-1, 1}, {0, 1}, {1, 1}, {2, 1}});
	blocks.push_back({{1, 0}, {1, -1}, {1, 1}, {1, 2}});
	blocks.push_back({{-1, 0}, {0, 0}, {1, 0}, {2, 0}});
}

char I_Shape::type() { return 'I'; }

T_Shape::T_Shape(int x, int y): Tetromino(x, y)
{
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {1, 0}});
}

char T_Shape::type() { return 'T'; }

S_Shape::S_Shape(int x, int y): Tetromino(x, y)
{
	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {-1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {0, 1}, {1, 1}});
	blocks.push_back({{0, 0}, {0, 1}, {1, -1}, {1, 0}});
	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {1, 0}});
}

char S_Shape::type() { return 'S'; }

Z_Shape::Z_Shape(int x, int y): Tetromino(x, y)
{
	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, 1}, {0, 1}, {1, 0}});
	blocks.push_back({{0, 0}, {0, -1}, {1, 0}, {1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {1, -1}});
}

char Z_Shape::type() { return 'Z'; }

J_Shape::J_Shape(int x, int y): Tetromino(x, y)
{
	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, 1}, {-1, 0}, {1, 0}});
	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, -1}});
}

char J_Shape::type() { return 'J'; }

L_Shape::L_Shape(int x, int y): Tetromino(x, y)
{
	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {-1, 1}});
	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, 1}});
	blocks.push_back({{0, 0}, {1, -1}, {0, -1}, {0, 1}});
	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {1, 0}});
}

char L_Shape::type() { return 'L'; }
