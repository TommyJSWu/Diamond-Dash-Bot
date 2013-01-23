#ifndef UTIL
#define UTIL

#include<windows.h>
#include "DiamondDashBot.h"

const int WindowW = 760, WindowH = 500, GridW = 40, GridH = 40;
const char dx[] = {-1, 0, 1, 0}, dy[] = {0, 1, 0, -1};

BOOL CALLBACK Proc(HWND window, LPARAM p);
void SplitRGB(unsigned, unsigned&, unsigned&, unsigned&);
bool FindGameWindow();
void GetRawBoard(unsigned**);
int GetBoard(DIAMOND**, unsigned**);
Move* GetMoves(DIAMOND**, int&);
void Select(int x, int y);
void ExecMove(int x, int y);

#endif
