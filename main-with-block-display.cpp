#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
//#include <utility>
#include <conio.h>
#include <windows.h>
#include <map>

using namespace std;

map <char, string> colorMap = {{'G', "cccccc"}, {'W', "ffffff"}, {'Z', "b20000"}, {'O', "e5e500"}, {'S', "007300"}, {'T', "b266b2"}, {'I', "00e1d0"}, {'L', "ffa500"}, {'J', "0000ff"}};

string Highlight(string color, string text)
{
    int r = stoi(color.substr(0, 2), nullptr, 16);
    int g = stoi(color.substr(2, 2), nullptr, 16);
    int b = stoi(color.substr(4, 2), nullptr, 16);
    
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + text + "\033[0m";
}

string SetColor(string color, string text)
{
    int r = stoi(color.substr(0, 2), nullptr, 16);
    int g = stoi(color.substr(2, 2), nullptr, 16);
    int b = stoi(color.substr(4, 2), nullptr, 16);

    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + text + "\033[0m";
}

void SetConsoleANSI()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD csMode = 0;
    GetConsoleMode(output, &csMode);
    csMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(output, csMode);
}


void GoTo(SHORT posY, SHORT posX)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

	SetConsoleCursorPosition(hStdout, Position);
}

void drawBlock(string color, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color, "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color, "   ");
}

void drawEmptyCell(string color, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color, "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color, "   ");
}

class Tetromino;

class Board {
private:
    const int rows;
    const int cols;
    vector<vector<char>> grid;

public:
    Board(int numRows, int numCols) : rows(numRows + 4), cols(numCols), grid(numRows + 4, vector<char>(numCols, ' ')) {}
	//Extra 4 rows for spawning/checking gameover
	void display() const
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
	int isInside(int x, int y){
		if (y < 0)
			return -1; // outer left
		else if (y >= cols)
			return 1; // outer right
		else if (x >= 0 && x < rows)
			return 2; // inside
		else
			return 0; // not inside
	}
	int getRow(){
		return rows;
	}
	int getCol(){
		return cols;
	}
	void clearRow(int x){
		for (int j = 0; j < cols; j++)
			grid[x][j] = ' ';
			
		// move all the above x lines down after clearing line x 	
		for (int i = x; i > 0; i--){
			for (int j = 0; j < cols; j++)
				grid[i][j] = grid[i-1][j];			
		}
	}
	bool checkClear(int pos, int &lines){
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
	char getCell(int x, int y){
		return grid[x][y];
	}
	bool isValid(){
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < cols; j++)
				if (grid[i][j] != ' ')
					return false;
		return true;
	}
	void addShape(Tetromino* tetromino);
};

// Tetromino class represents a single Tetris piece
class Tetromino {
protected:
	int state;
    pair <int, int> pos;
    vector <vector <pair <int, int> > > blocks;
public:
	Tetromino(int x, int y){
		state = 0;
		pos.first = x; 
		pos.second = y;
	}
    void move(int offSetX, int offSetY, Board& board){
    	for (int i = 0; i < blocks[state].size(); i++){
    		int u = pos.first + blocks[state][i].first + offSetX;
    		int v = pos.second + blocks[state][i].second + offSetY;
    		if (board.isInside(u, v) != 2 || board.getCell(u, v) != ' ')
    			return;
		}
    	
		pos.first += offSetX;
    	pos.second += offSetY;
	}
	vector <pair<int, int> > getBlocks(){
		return blocks[state];
	}
	pair<int, int> getPos(){
		return pos;
	}
	bool collisionCheck(Board& board){
		for (int i = 0; i < 4; i++){
			if (pos.first + blocks[state][i].first + 1  == board.getRow() || board.getCell(pos.first + blocks[state][i].first + 1, pos.second + blocks[state][i].second ) != ' '){
				return true;				
			}
		}
		return false;
	}
	void rotate(Board& board){
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
    void display(){
    	for (int i = 0; i < blocks[state].size(); i++){
    		int u = pos.first + blocks[state][i].first;
    		int v = pos.second + blocks[state][i].second;
    		drawBlock(colorMap[type()], u, v);
		}
	};
	void setPos(int x, int y){
		pos.first = x;
		pos.second = y;
	}
	virtual char type() = 0;
};

void Board::addShape(Tetromino* tetromino){
	pair <int, int> pos = tetromino->getPos();
	vector <pair <int, int> > blocks = tetromino->getBlocks();
	for (int i = 0; i < 4; i++){
		int u = pos.first + blocks[i].first;
		int v = pos.second + blocks[i].second;
		grid[u][v] = tetromino->type();
	}
}

class O_Shape : public Tetromino {
public:
    O_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, 1}, {1, 0}, {1, 1}});
	}
	char type()
	{
		return 'O';
	}
};

class I_Shape : public Tetromino {
public:
    I_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, -1}, {0, 0}, {0, 1}, {0, 2}});
    	blocks.push_back({{-1, 1}, {0, 1}, {1, 1}, {2, 1}});
    	blocks.push_back({{1, -1}, {1, 0}, {1, 1}, {1, 2}});
    	blocks.push_back({{-1, 0}, {0, 0}, {1, 0}, {2, 0}});
	}
	char type()
	{
		return 'I';
	}
};

class T_Shape : public Tetromino {
public: 
    T_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {1, 0}});
	}
	char type()
	{
		return 'T';
	}
};

class S_Shape : public Tetromino {
public:
    S_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {-1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {0, 1}, {1, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {1, -1}, {1, 0}});
    	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {1, 0}});
	}
	char type()
	{
		return 'S';
	}
};

class Z_Shape : public Tetromino {
public:
    Z_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, 1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {1, 0}, {1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {1, -1}});
	}
	char type()
	{
		return 'Z';
	}
};

class J_Shape : public Tetromino {
public:
    J_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, -1}});
	}
	char type()
	{
		return 'J';
	}
};

class L_Shape : public Tetromino {
public:
    L_Shape(int x = 0, int y = 0): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {-1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, 1}});
    	blocks.push_back({{0, 0}, {1, -1}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {1, 0}});
	}
	char type()
	{
		return 'L';
	}
};

class TetrisGame {
private:
    Board board;
    Tetromino* currentTetromino;
	vector <Tetromino*> tetrominoQueue;
	const time_t starttime; //Start time
	time_t nowtime; //Current time, only update once per second
	int clearedlines;  //Number of cleared lines in current level
	int score; 
	int level;	//A level requires 2*level + 1 lines to be cleared

public:
    TetrisGame(int numRows, int numCols) : board(numRows, numCols), currentTetromino(nullptr), starttime(time(0)), clearedlines(0) {}
	void displayTetrominoQueue(){
		for (int i = 0; i < tetrominoQueue.size(); i++){
			tetrominoQueue[i]->setPos(10*i + 5, 30);
			tetrominoQueue[i]->display();
		}
	}
    void spawnTetromino(int x, int y) {
    	while (tetrominoQueue.size() < 3){
			Tetromino* tempTetromino;
			int random = rand() % 7;
			switch (random){
				case 0:
					tempTetromino = new T_Shape;
					break;
				case 1: 
					tempTetromino = new I_Shape;
					break;
				case 2: 
					tempTetromino = new T_Shape;
					break;
				case 3: 
					tempTetromino = new S_Shape;
					break;
				case 4: 
					tempTetromino = new Z_Shape;
					break;
				case 5: 
					tempTetromino = new J_Shape;
					break;
				case 6: 
					tempTetromino = new L_Shape;
					break;	
			}
			tetrominoQueue.push_back(tempTetromino);
    	}
		currentTetromino = tetrominoQueue[0];
		currentTetromino->setPos(x, y);
		tetrominoQueue.erase(tetrominoQueue.begin());
	}
	void update_score(int newclearedlines) {
		clearedlines += newclearedlines;
		if (newclearedlines >= 1)
		{
			score += (newclearedlines - 1) * 200 + level * 100;
		}
	}

	bool gameOver() {
		return !board.isValid() || (nowtime - starttime >= 120);
	}

    void updateGame() {
		if (clearedlines == 2 * level + 1){
			level++;
			clearedlines = 0;
		}
    	if (currentTetromino == NULL)
    		spawnTetromino(0, 5);

		//Auto drop and clear lines
		time_t now = time(0);
		if (now - nowtime >= 1){
			if (currentTetromino->collisionCheck(board)){
				//Merge current shape into board
				board.addShape(currentTetromino);

				//Check for clear lines and update score
				int newclearedlines = 0;
				board.checkClear(currentTetromino->getPos().first-3, newclearedlines);
				update_score(newclearedlines);

				//Delete current shape
				delete currentTetromino;
				currentTetromino = NULL;
				
				return;
			}

			nowtime = now;
			currentTetromino->move(1, 0, board);
			displayGame();
		}

    	char input;
		if (kbhit())
		{
			input = getch();
			input = tolower(input);
			
			switch (input){
			case 's':
				currentTetromino->move(1, 0, board);
				break;
			case 'd':
				currentTetromino->move(0, 1, board);
				break;
			case 'a':
				currentTetromino->move(0, -1, board);
				break;
			case 'w': // rotate
				currentTetromino->rotate(board);
				break;
			case 'f': // drop
				while (!currentTetromino->collisionCheck(board))
					currentTetromino->move(1, 0, board);
				break;
			}
			displayGame();
			displayTetrominoQueue();
		}
    }

    void displayGame() const {
        board.display();
        if (currentTetromino != NULL)
        	currentTetromino->display();
    }
	void displayUI() const {
		
	}
};

int main() {
	srand(time(0)); 
    // Initialize and run the Tetris game
    TetrisGame game(20, 10);
	SetConsoleANSI();
	game.displayGame();
    while (true) {
        game.updateGame();
		if (game.gameOver())
			break;
    }

    return 0;
}
