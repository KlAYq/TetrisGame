#include "GameClass.h"

extern map <char, string> colorMap;

TetrisGame::TetrisGame(int numRows, int numCols) :
			board(numRows, numCols), currentTetromino(nullptr),
			score(0), level(1), starttime(time(0)),
			clearedlines(0) { time(&nowtime); }

void TetrisGame::spawnTetromino(int x, int y)
{
	while (tetrominoQueue.size() < 3)
	{
			Tetromino* tempTetromino;
			int random = rand() % 7;
			switch (random)
			{
				case 0: tempTetromino = new T_Shape; break;
				case 1: tempTetromino = new I_Shape; break;
				case 2: tempTetromino = new O_Shape; break;
				case 3: tempTetromino = new S_Shape; break;
				case 4: tempTetromino = new Z_Shape; break;
				case 5: tempTetromino = new J_Shape; break;
				case 6: tempTetromino = new L_Shape; break;	
			}
			tetrominoQueue.push_back(tempTetromino);
    	}
		currentTetromino = tetrominoQueue[0];
		currentTetromino->setPos(x, y);
		tetrominoQueue.erase(tetrominoQueue.begin());
}

void TetrisGame::update_score(int newclearedlines)
{
	clearedlines += newclearedlines;
	if (newclearedlines >= 1)
		score += (newclearedlines - 1) * 200 + level * 100;
}

bool TetrisGame::gameOver()
{
	return !board.isValid() || (nowtime - starttime >= 120);
}

void TetrisGame::updateGame()
{
	if (clearedlines == 2 * level + 1)
	{
		level++;
		clearedlines = 0;
	}
	if (currentTetromino == NULL)
		spawnTetromino(0, 5);

	//Auto drop and clear lines
	time_t now = time(0);
	if (now - nowtime >= 1)
	{
		if (currentTetromino->collisionCheck(board))
		{
			//Merge current shape into board
			board.addShape(currentTetromino);

			//Check for clear lines and update score
			int newclearedlines = 0;
			board.checkClear(currentTetromino->getPos().first-3, newclearedlines);
			update_score(newclearedlines);

			//Delete current shape
			delete currentTetromino;
			currentTetromino = NULL;
			PlaySound(TEXT("sfx/DropSFX01.wav"), NULL, SND_ASYNC | SND_FILENAME);
			return;
		}

		nowtime = now;
		currentTetromino->move(1, 0, board);
		displayGame();
		GoTo(16, 83);
		cout << score;
		GoTo(19, 83);
		cout << time(NULL) - starttime << "s";
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
		displayTetrominoQueue();
		displayGame();
	}
}

void TetrisGame::displayGame() const
{
    board.display();
    if (currentTetromino != NULL)
    	currentTetromino->display();
    displayTetrominoQueue();
}

void TetrisGame::displayTetrominoQueue() const
{
	for (int i = 2; i < 4; i++)
		for (int j = 26; j < 36; j++)
			drawEmptyCell("000000", i, j);
	for (int i = 0; i < tetrominoQueue.size(); i++)
	{
			tetrominoQueue[i]->setPos(5, 14 + 5 * i);
			tetrominoQueue[i]->display();
	}
}

void TetrisGame::displayUI() const 
{
	// Draw board border
	for (int i = BORDER_W + 2; i < BORDER_W + 26; i++)
		for (int j = BORDER_L - 2; j < BORDER_L + 12; j++)
			if (i == BORDER_W + 2|| i == BORDER_W + 25)
				drawBlock(colorMap['W'], i, j);
			else if (j == BORDER_L - 2|| j == BORDER_L + 11)
				drawBlock(colorMap['W'], i, j);
				
	// Draw game border
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23)
				drawBlock(colorMap['W'], i, j);
			else if (j == 0 || j == 37)
				drawBlock(colorMap['W'], i, j);	
	
	//Next piece - implemented else where
	GoTo(9, 83);
	cout << "^";
	GoTo(10, 83);
	cout << "Next block";
	for (int i = 26 ; i < 38; i++)
		drawBlock(colorMap['W'], 6, i);
	
	//Time
	GoTo(15, 83);
	cout << "Current Score: ";
	GoTo(18, 83);
	cout << "Time: ";
	GoTo(22, 83);
	cout << "[A] - Move Left";
	GoTo(24, 83);
	cout << "[D] - Move Right";
	GoTo(26, 83);
	cout << "[W] - Spin";
	GoTo(28, 83);
	cout << "[S] - Drop block";
	
	//Score

}

void TetrisGame::drawMenu() const
{
	//Dramatic border effect
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23)
			{
				Sleep(15);
				drawBlock(colorMap['W'], i, j);
			}
			else if (j == 0 || j == 37)
			{
				Sleep(15);
				drawBlock(colorMap['W'], i, j);	
			}
	
	//Game logo
	GoTo(10, 10);
	ifstream logo("img/logo2.txt");
	string line;
	int iline = 0;
	while(getline(logo, line))
	{
		Sleep(15);
		for (int i = 0; i < line.length(); i++)
			if (line[i] == ' ')
				drawBlock("000000", 2 + iline, 4 + i);
			else
				drawBlock(colorMap[char(line[i])], 2 + iline, 4 + i);
		iline++;
	}
	logo.close();
	//Buttons
	
	GoTo(28,30);
	cout << "Welcome!";
	
	GoTo(30,30);
	cout << "<START GAME>";
	GoTo(32,30);
	cout << "<HOW TO PLAY>";
	GoTo(34,30);
	cout << "<MORE>";
	GoTo(36,30);
	cout << "<QUIT>";
	
	
	bool inMenuMode = true;
	int option = 0;
	
	while(inMenuMode){		
		GoTo(30,45);
		cout << "   ";
		GoTo(32,45);
		cout << "   ";
		GoTo(34,45);
		cout << "   ";
		GoTo(36,45);
		cout << "   ";
		
		GoTo(30 + 2*option, 45);
		cout << "<<";
		
		char c = _getch();
		switch (tolower(c)){
			case 's': option++; option %= 4; break;
			case 'w': option--; option = option < 0 ? 3 : option; break;
			case 13: inMenuMode = false; break;
			default: break;
		}
	}
	
	for (int i = 1; i < 23; i++)
		for (int j = 1; j < 32; j++)
			drawEmptyCell("000000", i, j);
}

