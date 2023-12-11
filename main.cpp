#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
//#include <utility>
#include <conio.h>
#include <windows.h>

using namespace std;

void GoTo(SHORT posY, SHORT posX)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

	SetConsoleCursorPosition(hStdout, Position);
}

class Tetromino;

class Board {
private:
    const int rows;
    const int cols;
    vector<vector<char>> grid;

public:
    Board(int numRows, int numCols) : rows(numRows), cols(numCols), grid(numRows, vector<char>(numCols, ' ')) {}

    void display() const {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                cout << grid[i][j];
            }
           	cout << '\n';
        }
    }
	void enableCell(int x, int y){
		grid[x][y] = '0';
	}
	void disableCell(int x, int y){
		grid[x][y] = ' ';
	}
	bool isInside(int x, int y){
		return x >= 0 && x < rows && y >= 0 && y < cols;
	}
	int getRow(){
		return rows;
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
	void checkClear(int pos){
		for (int i = pos; i < rows; i++){
			int isComplete = true;
			for (int j = 0; j < cols; j++)
				if (grid[i][j] == ' '){
					isComplete = false;
				}
			if (isComplete)
				clearRow(i);
		}
	}
	char getCell(int x, int y){
		return grid[x][y];
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
	Tetromino(int& x, int& y){
		state = 0;
		pos.first = x; 
		pos.second = y;
	}
    void move(int offSetX, int offSetY, Board& board){
    	for (int i = 0; i < blocks[state].size(); i++){
    		int u = pos.first + blocks[state][i].first + offSetX;
    		int v = pos.second + blocks[state][i].second + offSetY;
    		if (!board.isInside(u, v) || board.getCell(u, v) != ' ')
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
			if ( pos.first + blocks[state][i].first + 1  == board.getRow() || board.getCell(pos.first + blocks[state][i].first + 1, pos.second + blocks[state][i].second ) != ' '){
				return true;				
			}
		}
		return false;
	}
	void rotate(Board& board){
		int tempState = (state + 1) % blocks.size();
		for (int i = 0; i < blocks[tempState].size(); i++){
    		int u = pos.first + blocks[tempState][i].first;
    		int v = pos.second + blocks[tempState][i].second;
    		if (!board.isInside(u, v) || board.getCell(u, v) != ' ')
    			return;
		}
		
		state++;
		state %= blocks.size();
	}
    void display(){
    	for (int i = 0; i < blocks[state].size(); i++){
    		int u = pos.first + blocks[state][i].first;
    		int v = pos.second + blocks[state][i].second;
    		GoTo(u, v);
    		cout << "0";
		}
	};
};

void Board::addShape(Tetromino* tetromino){
		pair <int, int> pos = tetromino->getPos();
		vector <pair <int, int> > blocks = tetromino->getBlocks();
		for (int i = 0; i < 4; i++){
			int u = pos.first + blocks[i].first;
    		int v = pos.second + blocks[i].second;
			grid[u][v] = '0';
		}
	}

class O_Shape : public Tetromino {
public:
    O_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, 1}, {1, 0}, {1, 1}});
	}
};

class I_Shape : public Tetromino {
public:
    I_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, -1}, {0, 0}, {0, 1}, {0, 2}});
    	blocks.push_back({{-1, 1}, {0, 1}, {1, 1}, {2, 1}});
    	blocks.push_back({{1, -1}, {1, 0}, {1, 1}, {1, 2}});
    	blocks.push_back({{-1, 0}, {0, 0}, {1, 0}, {2, 0}});
	}
};

class T_Shape : public Tetromino {
public:
    T_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {1, 0}});
	}
};

class S_Shape : public Tetromino {
public:
    S_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, -1}, {-1, 0}, {-1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {0, 1}, {1, 1}});
    	blocks.push_back({{0, 0}, {0, 1}, {1, -1}, {1, 0}});
    	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {1, 0}});
	}
};

class Z_Shape : public Tetromino {
public:
    Z_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, 1}, {0, 1}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {1, 0}, {1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {0, -1}, {1, -1}});
	}
};

class J_Shape : public Tetromino {
public:
    J_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {-1, -1}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, 1}, {-1, 0}, {1, 0}});
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, -1}});
	}
};

class L_Shape : public Tetromino {
public:
    L_Shape(int x, int y): Tetromino(x, y){
    	blocks.push_back({{0, 0}, {0, -1}, {0, 1}, {-1, 1}});
    	blocks.push_back({{0, 0}, {-1, 0}, {1, 0}, {1, 1}});
    	blocks.push_back({{0, 0}, {1, -1}, {0, -1}, {0, 1}});
    	blocks.push_back({{0, 0}, {-1, -1}, {-1, 0}, {1, 0}});
	}
};




class TetrisGame {
private:
    Board board;
    Tetromino* currentTetromino;

public:
    TetrisGame(int numRows, int numCols) : board(numRows, numCols), currentTetromino(nullptr) {}

    void spawnTetromino(int x, int y) {
    	int random = rand() % 7;
    	switch (random){
    		case 0:
    			currentTetromino = new O_Shape(x, y);
    			break;
    		case 1: 
    			currentTetromino = new I_Shape(x, y);
    			break;
    		case 2: 
    			currentTetromino = new T_Shape(x, y);
    			break;
    		case 3: 
    			currentTetromino = new S_Shape(x, y);
    			break;
    		case 4: 
    			currentTetromino = new Z_Shape(x, y);
    			break;
    		case 5: 
    			currentTetromino = new J_Shape(x, y);
    			break;
    		case 6: 
    			currentTetromino = new L_Shape(x, y);
    			break;	
		}
    	
    }

    void updateGame() {
    	if (currentTetromino == NULL)
    		spawnTetromino(0, 5);
    	char input;
    	input = getch();
		
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
		if (currentTetromino->collisionCheck(board)){
			board.addShape(currentTetromino);
			board.checkClear(currentTetromino->getPos().first-3);
			delete currentTetromino;
			currentTetromino = NULL;
		}	
		
    }

    void displayGame() const {
        system("cls");
        board.display();
        if (currentTetromino != NULL)
        	currentTetromino->display();
    }
};

int main() {
	srand(time(0)); 
    // Initialize and run the Tetris game
    TetrisGame game(20, 10);

    while (true) {
        game.updateGame();
        game.displayGame();

    }

    return 0;
}
