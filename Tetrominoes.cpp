#include "Tetrominoes.h"
extern map <char, string> colorMap;
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
	for (int i = 0; i < blocks[state].size(); i++)
	{
		if (pos.first + blocks[state][i].first + 1  == board.getRow() || board.getCell(pos.first + blocks[state][i].first + 1, pos.second + blocks[state][i].second ) != ' '){
			return true;				
		}
	}
	return false;
}

void Tetromino::rotate(Board& board){
	int tempState = (state + 1) % blocks.size();

	int I_shapeOffset = 0; // Because the positions of the blocks in Tetromino 'I' are offset by 1 in some states, we must include this variable in the calculation to be able to behave like other Tetromino.
		if (type() == 'I' && (tempState == 1 || tempState == 2))
			I_shapeOffset = 1;
	pair <int, int> moveDir = {0, 0};
	
	// Check in advance whether the position after rotation is valid or not to have the appropriate move direction
	for (int i = 0; i < blocks[tempState].size(); i++)
	{
		int u = pos.first + blocks[tempState][i].first;
		int v = pos.second + blocks[tempState][i].second;
		if ((board.isInside(u + moveDir.first, v + moveDir.second) != 2) || (board.isInside(u + moveDir.first, v + moveDir.second) == 2 && board.getCell(u + moveDir.first, v + moveDir.second) != ' ')){ // one of the block hit the other block -> move up/left/right accordingly
				if (pos.second + I_shapeOffset > v) 		// move rightward
						moveDir.second++;
				else if (pos.second + I_shapeOffset < v) 	// move leftward 
						moveDir.second--;
				else 										// move upward
					moveDir.first--;
			}
	}
	// Even after moving, there is still an obstacle, the Tetromino will not move or rotate by return the function
	for (int i = 0; i < blocks[tempState].size(); i++)
	{
		int u = pos.first + blocks[tempState][i].first + moveDir.first;
		int v = pos.second + blocks[tempState][i].second + moveDir.second;
		if ((board.isInside(u, v) != 2) ||(board.isInside(u, v) == 2 && board.getCell(u, v) != ' ')) 
			return;
	}

	// else we move the Tetromino and rotate it
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
