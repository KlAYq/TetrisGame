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


void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width - 1;
    WindowSize.Bottom = height - 1;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}

void DisableResizeWindow()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void ShowScrollbarOption(BOOL Show)
{
    HWND hWnd = GetConsoleWindow();
    ShowScrollBar(hWnd, SB_BOTH, Show);
}

void DisableCtrButton(bool Close, bool Min, bool Max)
{
    HWND hWnd = GetConsoleWindow();
    HMENU hMenu = GetSystemMenu(hWnd, false);

    if (Close == 1)
    {
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (Min == 1)
    {
        DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
    }
    if (Max == 1)
    {
        DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
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
void drawShadedBlock(string color, int Y, int X)
{
    GoTo(Y * 2, X * 3);
    char c = 176;
    string s(3, c);
    cout << Highlight(color, s);
    GoTo(Y * 2 + 1, X * 3);
    cout << Highlight(color, s);
}

void ShowConsoleCursor(bool showFlag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo);
}