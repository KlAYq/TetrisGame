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
	return !board.isValid() || (nowtime - starttime >= 10);
}

void TetrisGame::updateGame()
{
	if (clearedlines == 2 * level + 1)
	{
		level++;
		clearedlines = 0;
	}
	if (currentTetromino == NULL)
	{
		spawnTetromino(3, 5);
		currentTetromino->display();
		displayTetrominoQueue();
	}
	//Auto drop and clear lines
	time_t now = time(0);
	if (now - nowtime >= 1)
	{
		if (currentTetromino->collisionCheck(board))
		{
			//Merge current shape into board
			board.addShape(currentTetromino);
			board.display();
			//Check for clear lines and update score
			int newclearedlines = 0;
			board.checkClear(currentTetromino->getPos().first - 3, newclearedlines);
			update_score(newclearedlines);
			board.display();
			//Delete current shape
			delete currentTetromino;
			currentTetromino = NULL;
			return;
		}

		nowtime = now;
		currentTetromino->move(1, 0, board);
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
	}
}

void TetrisGame::displayGame() const
{
    board.display();
    if (currentTetromino != NULL)
    	currentTetromino->display();
    displayTetrominoQueue();
}

void TetrisGame::ResetGame(){
	board.ResetBoard();
	while (tetrominoQueue.size()){
		delete tetrominoQueue[0];
		tetrominoQueue.erase(tetrominoQueue.begin());
	}
	if (currentTetromino)
		delete currentTetromino;
	currentTetromino = NULL;
}

void TetrisGame::displayTetrominoQueue() const
{
	for (int i = 2; i < 4; i++)
		for (int j = 26; j < 36; j++)
			drawBlock("000000", i, j);
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
	
	//Controls
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
	
	char colorInputs[7] = "TOSJIL";
	
	ifstream imgf("img/vlogo.txt");
	string line;
	int iline = 0;
	int r = rand() % 6;
	while(getline(imgf, line))
	{
		for (int i = 0; i < line.length(); i++)
			if (line[i] == ' ')
				drawBlock("000000", 3 + iline, 2 + i);
			else
				drawBlock(colorMap[colorInputs[(int(line[i] - '1') + r) % 6]], 3 + iline, 2 + i);
		iline++;
	}
	imgf.close();
}

void TetrisGame::drawHowToPlay() const
{
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23)
			{ drawBlock(colorMap['W'], i, j); }
			else if (j == 0 || j == 37)
			{ drawBlock(colorMap['W'], i, j); }
	
	ifstream im("img/howtoplay.txt");
	string imline;
	int itline = 0;
	while(getline(im, imline))
	{
		for (int i = 0; i < imline.length(); i++)
			if (imline[i] == ' ')
				drawBlock("000000", 14 + itline, 24 + i);
			else
				drawBlock(colorMap[char(imline[i])], 14 + itline, 24 + i);
		itline++;
	}
	im.close();
	
	GoTo(8, 18);
	cout << "I";
	GoTo(13, 18);
	cout << "O";
	GoTo(19, 18);
	cout << "T";
	GoTo(7, 47);
	cout << "Z";
	GoTo(13, 47);
	cout << "S";
	GoTo(19, 47);
	cout << "L";
	GoTo(25, 47);
	cout << "J";
	GoTo(30, 24);
	cout << "7 types of blocks.";
	GoTo(31, 27);
	cout << "(tet means 4)";
	
	GoTo(19, 70);
	cout << "Form a line to remove them";
	GoTo(43, 67);
	cout << "Try not to fill the pieces up top";
	
	I_Shape(6, -6).display();
	O_Shape(9, -6).display();
	T_Shape(12, -6).display();
	Z_Shape(6, -1).display();
	S_Shape(9, -1).display();
	L_Shape(12, -1).display();
	J_Shape(15, -1).display();
	
	GoTo(43, 17);
	cout << "<Press Enter to return to Main Menu>";
	
	// Drawing animation
	bool running = true;
	time_t t1, t2 = time(0);
	int it = 0;
	while (running)
	{
		t1 = time(0);
		if (kbhit()){
			char input;
			input = getch();
			if (input == 13){
				running = false;
				break;
			}
		}
		if (t1 - t2 >= 1){
			ifstream imgf("img/howtoplay_" + to_string(it) + ".txt");
			string line;
			int iline = 0;
			while(getline(imgf, line))
			{
				for (int i = 0; i < line.length(); i++)
					if (line[i] == ' ')
						drawBlock("000000", 3 + iline, 24 + i);
					else
						drawBlock(colorMap[char(line[i])], 3 + iline, 24 + i);
				iline++;
			}
			imgf.close();
			t2 = t1;
			++it %= 8;
		}
	}
	
}

void TetrisGame::drawCredits() const{
	
	for (int i = 0; i < 24; i++)
		for (int j = 0; j < 38; j++)
			if (i == 0 || i == 23)
			{
				drawBlock(colorMap['W'], i, j);
			}
			else if (j == 0 || j == 37)
			{
				drawBlock(colorMap['W'], i, j);	
			}
	
	ifstream im("img/amongus.txt");
	string imline;
	int itline = 0;
	while(getline(im, imline))
	{
		for (int i = 0; i < imline.length(); i++)
			if (imline[i] == ' ')
				drawBlock("000000", 3 + itline, 12 + i);
			else if (imline[i] == '0')
				drawBlock(colorMap['I'], 3 + itline, 12 + i);
			else if (imline[i] == '1')
				drawBlock("ff0000", 3 + itline, 12 + i);
		itline++;
	}
	im.close();
	
	GoTo(12, 70);
	cout << "Meet the creators:";
	
	GoTo(14, 70);
	cout << "22127028 - Ha \"KanCh\" Gia Bao";
	GoTo(15, 70);
	cout << "22127200 - Vo \"FakeMonika\" Dang Khoa";
	GoTo(16, 70);
	cout << "22127258 - Le \"KLAY\" Tri Man";
	GoTo(17, 70);
	cout << "22127452 - Le \"Shadow\" Ngoc Vi";
	
	
	GoTo(21, 70);
	cout << "Special thanks to the teachers: ";
	GoTo(23, 70);
	cout << "Mr. Bui Tien Len";
	GoTo(24, 70);
	cout << "Mr. Truong Tan Khoa";
	GoTo(25, 70);
	cout << "Mr. Le Thanh Phong";
	
	GoTo(43, 67);
	cout << "<Press Enter to return to Main Menu>";
	
	char colorInputs[7] = "TOSJIL";
	
	bool running = true;
	time_t t1, t2 = time(0);
	int it = 0;
	while (running){
		t1 = time(0);
		if (kbhit()){
			char input;
			input = getch();
			if (input == 13){
				running = false;
				break;
			}
		}
		if (t1 - t2 >= 1){
			ifstream imgf("img/vlogo.txt");
			string line;
			int iline = 0;
			while(getline(imgf, line))
			{
				for (int i = 0; i < line.length(); i++)
					if (line[i] == ' ')
						drawBlock("000000", 3 + iline, 2 + i);
					else
						drawBlock(colorMap[colorInputs[(int(line[i] - '1') + it) % 6]], 3 + iline, 2 + i);
				iline++;
			}
			imgf.close();
			t2 = t1;
			++it %= 6;
		}
	}
}

void TetrisGame::drawGameOver() const{
	GoTo(20, 83);
	cout << "Game over!";
	
	ifstream imgf("img/vlogo.txt");
	string line;
	int iline = 0;
	while(getline(imgf, line))
	{
		for (int i = 0; i < line.length(); i++)
			if (line[i] == ' ')
				drawBlock("000000", 3 + iline, 2 + i);
			else
				drawBlock("ff0000", 3 + iline, 2 + i);
		iline++;
	}
	imgf.close();
	
	Sleep(2000);
	for (int i = 21; i >= 2; i--){
		Sleep(25);
		for (int j = 14; j < 24; j++)
			drawBlock("000000", i, j);
	}
	Sleep(2000);
}

int TetrisGame::drawMenu() const
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
	
	while(inMenuMode)
	{		
		GoTo(30,45);
		cout << "   ";
		GoTo(32,45);
		cout << "   ";
		GoTo(34,45);
		cout << "   ";
		GoTo(36,45);
		cout << "   ";
		
		GoTo(30 + 2 * option, 45);
		cout << "<<";
		
		char c = _getch();
		switch (tolower(c))
		{
			case 's': option++; option %= 4;
					  PlaySound(TEXT("sfx/Select.wav"), NULL, SND_ASYNC | SND_FILENAME); break;
			case 'w': option--; option = option < 0 ? 3 : option;
					  PlaySound(TEXT("sfx/Select.wav"), NULL, SND_ASYNC | SND_FILENAME); break;
			case 13:  inMenuMode = false;
					  PlaySound(TEXT("sfx/Select.wav"), NULL, SND_ASYNC | SND_FILENAME); break;
			default: break;
		}
		
	}
	for (int i = 1; i < 23; i++)
		for (int j = 1; j < 32; j++)
			drawBlock("000000", i, j);
	return option;
}

void TetrisGame::GameInit()
{
	bool game_running = true;
	while (game_running){
		system("cls");
		int option = drawMenu();
		switch (option)
		{
			case 0: 
				ResetGame();
				displayGame();
				displayUI();
				starttime = time(0);
				while (true) {
		        	updateGame();
					if (gameOver()) break;
			    }
			    // Game over screen
			    drawGameOver();	break;
		    case 1: drawHowToPlay(); break;
		    case 2: drawCredits(); break;
		    case 3: game_running = false; break;
		    default: break;
		}	
	}
}
