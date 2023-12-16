#pragma once
#include <iostream>
#include <windows.h>
#include <Windows.h>
#include <unistd.h>
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
void SetWindowSize(SHORT width, SHORT height);         //Resize console
void SetScreenBufferSize(SHORT width, SHORT height);   //Fit screen buffer
void DisableResizeWindow(); 						   //Locks the dimensions of the console
void ShowScrollbarOption(BOOL Show);                   //Show scrollbar or not
void DisableCtrButton(bool Close, bool Min, bool Max); //Disable expand, control buttons
void drawBlock(string color, int Y, int X);
void drawShadedBlock(string color, int Y, int X);
void ShowConsoleCursor(bool showFlag);

