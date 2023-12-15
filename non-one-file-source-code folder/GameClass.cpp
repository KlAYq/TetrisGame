#include "GameClass.h"
#include "Tetrominoes.h"

TetrisGame::TetrisGame(int numRows, int numCols) : board(numRows, numCols), currentTetromino(nullptr), starttime(time(0)), clearedlines(0) {}

void TetrisGame::spawnTetromino(int x, int y) {
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
					tempTetromino = new O_Shape;
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
		displayTetrominoQueue();
	}
}

void TetrisGame::displayGame() const {
    board.display();
    if (currentTetromino != NULL)
    	currentTetromino->display();
}

void TetrisGame::displayTetrominoQueue(){
	for (int i = 2; i < 4; i++)
		for (int j = 26; j < 36; j++)
			drawEmptyCell("000000", i, j);
	for (int i = 0; i < tetrominoQueue.size(); i++){
			tetrominoQueue[i]->setPos(5, 14 + 5 * i);
			tetrominoQueue[i]->display();
	}
}

void TetrisGame::displayUI() const {
	// Draw board border
	for (int i = BORDER_W + 2; i < BORDER_W + 26; i++)
		for (int j = BORDER_L - 2; j < BORDER_L + 12; j++)
			if (i == BORDER_W + 2|| i == BORDER_W + 25)
				drawEmptyCell(colorMap['W'], i, j);
			else if (j == BORDER_L - 2|| j == BORDER_L + 11)
				drawEmptyCell(colorMap['W'], i, j);
				
	// Draw game border
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23)
				drawEmptyCell(colorMap['W'], i, j);
			else if (j == 0 || j == 37)
				drawEmptyCell(colorMap['W'], i, j);	
	
	//Next piece - implemented else where
	GoTo(10, 89);
	cout << "Next block";
	for (int i = 26 ; i < 38; i++)
		drawEmptyCell(colorMap['W'], 6, i);
	
	//Time
	
	//Score

}

void TetrisGame::drawMenu() const {
	//Dramatic border effect
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23){
				Sleep(25);
				drawEmptyCell(colorMap['W'], i, j);
			}
			else if (j == 0 || j == 37){
				Sleep(25);
				drawEmptyCell(colorMap['W'], i, j);	
			}
	
	//Game logo
	GoTo(10, 10);
	ifstream logo("img/logo2.txt");
	string line;
	int iline = 0;
	while(getline(logo, line)){
		Sleep(25);
		for (int i = 0; i < line.length(); i++)
			if (line[i] == ' ')
				drawEmptyCell("000000", 2 + iline, 4 + i);
			else
				drawBlock(colorMap[char(line[i])], 2 + iline, 4 + i);
		iline++;
	}
	logo.close();
	//Buttons
	
	GoTo(30,50);
	
	cout << "Press Enter to play";
	_getch();
	
	for (int i = 1; i < 23; i++)
		for (int j = 1; j < 32; j++)
			drawEmptyCell("000000", i, j);
}

