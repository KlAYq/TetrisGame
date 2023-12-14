#pragma once
#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <map>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

using namespace std;

string Highlight(string color, string text);
string SetColor(string color, string text);
void SetConsoleANSI();
void GoTo(SHORT posY, SHORT posX);
void drawBlock(string color, int Y, int X);
void drawEmptyCell(string color, int Y, int X);
