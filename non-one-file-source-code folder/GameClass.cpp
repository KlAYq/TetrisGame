#include "GameClass.h"
#include "Tetrominoes.h"

TetrisGame::TetrisGame(int numRows, int numCols) : board(numRows, numCols), currentTetromino(nullptr), starttime(time(0)), clearedlines(0) {}

void TetrisGame::spawnTetromino(int x, int y) {
	int random = rand() % 7;
	switch (random){
		case 0:
			currentTetromino = new T_Shape(x, y);
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

void TetrisGame::update_score(int newclearedlines) {
	clearedlines += newclearedlines;
	if (newclearedlines >= 1)
	{
		score += (newclearedlines - 1) * 200 + level * 100;
	}
}

bool TetrisGame::gameOver() {
	return !board.isValid() || (nowtime - starttime >= 120);
}

void TetrisGame::updateGame() {
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
	}
}

void TetrisGame::displayGame() const {
    board.display();
    if (currentTetromino != NULL)
    	currentTetromino->display();
}
void TetrisGame::displayUI() const {
	
}

