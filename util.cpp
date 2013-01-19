#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

using std::string;

struct DC_s
{
    HWND hwnd;
    unsigned width, height;
    HDC DC, memDC;
    HBITMAP hbmp;
} GameWindow;

unsigned *buf;

int WindowW = 760, WindowH = 500;

BOOL CALLBACK Proc(HWND window, LPARAM p = NULL)
{
    if(GameWindow.hwnd != NULL ) return false;
    RECT rect;
    GetWindowRect(window, &rect);
    int width   = rect.right - rect.left,
        height  = rect.bottom - rect.top;

    //if(rect.left*rect.right*rect.top*rect.bottom!=0)
    //printf("%d %d %d %d ", rect.left, rect.right, rect.top, rect.bottom);
    //if(rect.left <=0  || width < 700) return true;

    if(height < WindowH || width < WindowW) return true;

    char buf[99];
    GetClassName(window, buf, sizeof(buf));
    //if(!strncmp(buf, "Chrome", 6))
    string tmp = "WindowClass";
    string tmp2 = buf;
    if(width==760 && tmp2.find(tmp)!=string::npos)
    {
        printf("%d %d | %d %d\n", width, height, rect.left, rect.right);
        printf("=======================================%s", buf);
        puts("");
    }
    if(width == WindowW && height == WindowH && !strcmp(buf, "NativeWindowClass"))
    {
        GameWindow.hwnd = window;
        return false;
    }
    EnumChildWindows(window, Proc, (LPARAM)NULL);
    return true;
}

bool FindGameWindow()
{
    EnumWindows(Proc, (LPARAM)NULL);
    if(GameWindow.hwnd == NULL) return false;
    RECT rect;
    GetWindowRect(GameWindow.hwnd, &rect);
    GameWindow.height = rect.bottom - rect.top;
    GameWindow.width = rect.right - rect.left;
    GameWindow.DC = GetDC(GameWindow.hwnd);
    GameWindow.memDC = CreateCompatibleDC(GameWindow.DC);
    GameWindow.hbmp = CreateCompatibleBitmap(GameWindow.DC, GameWindow.width, GameWindow.height);
    SelectObject(GameWindow.memDC, GameWindow.hbmp);
    unsigned BmpSize = GameWindow.width * GameWindow.height * 4;
    buf = new unsigned[BmpSize/4];
    return true;
}
