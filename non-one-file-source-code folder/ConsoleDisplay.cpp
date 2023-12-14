#pragma once
#include "ConsoleDisplay.h"

// Color configuration of blocks using HEX colors
map <char, string> colorMap = {{'G', "cccccc"}, {'W', "ffffff"}, {'Z', "b20000"}, {'O', "e5e500"}, {'S', "007300"}, {'T', "b266b2"}, {'I', "00e1d0"}, {'L', "ffa500"}, {'J', "0000ff"}};

// Drawing background (lower layer)
string Highlight(string color, string text)
{
    int r = stoi(color.substr(0, 2), nullptr, 16);
    int g = stoi(color.substr(2, 2), nullptr, 16);
    int b = stoi(color.substr(4, 2), nullptr, 16);
    
    return "\033[48;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + text + "\033[0m";
}

// Drawing text (upper layer)
string SetColor(string color, string text)
{
    int r = stoi(color.substr(0, 2), nullptr, 16);
    int g = stoi(color.substr(2, 2), nullptr, 16);
    int b = stoi(color.substr(4, 2), nullptr, 16);

    return "\033[38;2;" + to_string(r) + ";" + to_string(g) + ";" + to_string(b) + "m" + text + "\033[0m";
}

// Console configuration
void SetConsoleANSI()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD csMode = 0;
    GetConsoleMode(output, &csMode);
    csMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(output, csMode);
}

// Move mouse/console pointer
void GoTo(SHORT posY, SHORT posX)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

	SetConsoleCursorPosition(hStdout, Position);
}

// Draw colored squares
void drawBlock(string color, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color, "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color, "   ");
}

// Draw non-colored squares (?)
void drawEmptyCell(string color, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    cout << Highlight(color, "   ");
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color, "   ");
}

