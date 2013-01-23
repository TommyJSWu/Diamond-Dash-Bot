#define _WIN32_WINNT 0x500
#include "util.h"
#include "DiamondDashBot.h"
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

    if(width == WindowW && height == WindowH)
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

inline unsigned PixelCoorToBmpPos(int y, int x)
{
    return (MarginY - 43 + y) * WindowW + MarginX + x;
}

void GetRawBoard(unsigned **r)
{

    BitBlt(GameWindow.memDC,
           0,
           0,
           GameWindow.width,
           GameWindow.height,
           GameWindow.DC,
           0, 
           0,
           SRCCOPY);

    BITMAPINFOHEADER bmpHeader;
    bmpHeader.biSize            = sizeof(BITMAPINFOHEADER);
    bmpHeader.biWidth           = GameWindow.width;
    bmpHeader.biHeight          = GameWindow.height;
    bmpHeader.biPlanes          = 1;
    bmpHeader.biBitCount        = 32;
    bmpHeader.biCompression     = BI_RGB;
    bmpHeader.biSizeImage       = 0;
    bmpHeader.biXPelsPerMeter   = 0;
    bmpHeader.biYPelsPerMeter   = 0;
    bmpHeader.biClrUsed         = 0;
    bmpHeader.biClrImportant    = 0;

   
    unsigned BmpSize = GameWindow.width * GameWindow.height * 4;
    
    GetDIBits(GameWindow.memDC, GameWindow.hbmp, 0, (UINT)GameWindow.height, buf, (BITMAPINFO*)&bmpHeader, DIB_RGB_COLORS);

    int i, j;
    unsigned *tmpbuf = new unsigned[BoardH*GridH*BoardW*GridW];
    //unsigned *tmpbuf = new unsigned[BmpSize/4];
    for(i=0;i<BoardH*GridH; i++)
        for(j=0;j<BoardW*GridW; j++)
        {
            r[BoardH*GridH - i - 1][j] = buf[PixelCoorToBmpPos(i, j)] & 0x00FFFFFF;
            //tmpbuf[i*BoardW*GridW + j] = buf[PixelCoorToBmpPos(i, j)];
            //tmpbuf[i*BoardH*GridH + j] = buf[i*BoardH*GridH + j];
        }
    
    /*
    BmpSize = BoardH*GridH*BoardW*GridW * 4;
  bmpHeader.biWidth = BoardW * GridW;
  bmpHeader.biHeight = BoardH * GridH;
  puts("output bmpfile");
  HANDLE hFile = CreateFile("captureqwsx.bmp",
      GENERIC_WRITE,
      0,
      NULL,
      CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL, NULL);   

  BITMAPFILEHEADER   bmfHeader;
  DWORD dwSizeofDIB = BmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
  bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER); 
  bmfHeader.bfSize = dwSizeofDIB; 
  bmfHeader.bfType = 0x4D42;
  DWORD dwBytesWritten = 0;
  WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
  WriteFile(hFile, (LPSTR)&bmpHeader, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
  WriteFile(hFile, (LPSTR)tmpbuf, BmpSize, &dwBytesWritten, NULL);
  CloseHandle(hFile);
  delete []tmpbuf;
 
  */
  
    
}

void SplitRGB(unsigned color, unsigned &r, unsigned &g, unsigned &b)
{
    r = color >> 16;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
}

int GetBoard(DIAMOND **board, unsigned **rawBoard)
{
    GetRawBoard(rawBoard);
    int i, j, x, y;
    unsigned r, g, b;
    int count[8], tmp, max;
    for(i=0;i<BoardH;i++)
    {
        for(j=0;j<BoardW;j++)
        {
            for(x=0;x<8;x++) count[x] = 0;
            for(x=6;x<=33;x++)
                for(y=6;y<=33;y++)
                {
                    SplitRGB(rawBoard[i * GridH + x][j * GridW + y], r, g, b);
                    tmp = ((r>>7)<<2) | ((g>>7)<<1) | (b>>7); //MSB of rgb
                    count[tmp] ++;
                }
            for(x=max=0;x<8;x++)
                if(count[x] > count[max]) max = x;
            if(max == 4) // 100
                board[i][j] = RED;
            else if(max == 2) // 010
                board[i][j] = GREEN;
            else if(max == 1) // 001
                board[i][j] = BLUE;
            else if(max == 6) // 110
                board[i][j] = YELLOW;
            else if(max == 5) // 101
                board[i][j] = PURPLE;
            else if(max == 3 || count[3] + count[6] + 25 > count[7] && count[3] > 0.35 * count[7] && count[6] < 0.65 * count[7]) // 011
                board[i][j] = SUPER;
            else 
                board[i][j] = ERRORD;
            /*
            if(max == 7)
            {
                printf("Now at: %d %d\n", i, j);
                for(x=0;x<8;x++) printf("%d ", count[x]);
                puts("x");
            }
            */
            //printf("%d ", count[max]);
            //printf("%d ", max);
        }
        //puts("");
    }
    int errCount = 0;
    for(i=0;i<BoardH;i++)
        for(j=0;j<BoardW;j++)
            errCount += board[i][j] == ERRORD;
    return errCount;
}

bool used[BoardH][BoardW];

char floodFill(int x, int y, DIAMOND color, DIAMOND** board)
{
    char i, sum = 1;
    used[x][y] = 1;
    for(i=0;i<4;i++)
        if(x + dx[i] >= 0 && x + dx[i] < BoardH && y + dy[i] >= 0 && y + dy[i] < BoardW 
           && board[x+dx[i]][y+dy[i]] == color && !used[x+dx[i]][y+dy[i]])
            sum += floodFill(x + dx[i], y + dy[i], color, board);
    return sum;
}

Move* GetMoves(DIAMOND **board, int &nMoves)
{
    Move *moves = new Move[90];
    nMoves = 0;
    char i, j;
    for(i=0;i<BoardH;i++)
        for(j=0;j<BoardW;j++)
            used[i][j] = 0;
    for(i=0;i<BoardH;i++)
        for(j=0;j<BoardW;j++)
            if(!used[i][j])
                if(board[i][j] != ERRORD && (board[i][j] == SUPER || floodFill(i, j, board[i][j], board) >= 3))
                    moves[nMoves++] = (Move){i, j, board[i][j]};
    return moves;
}

inline int transX(int x)
{
    return MarginY + x * GridH + GridH / 2;
}

inline int transY(int y)
{
    return MarginX + y * GridW + GridW / 2;
}

void Select(int x, int y)
{
    x = transX(x);
    y = transY(y);
    SendNotifyMessage(GameWindow.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(y, x));
    SendNotifyMessage(GameWindow.hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(y, x));
}

void ExecMove(int x, int y)
{
    x = transX(x);
    y = transY(y);
    //printf("Exec: %d %d \n", y, x);
    RECT rect;
    POINT pt;
    pt.x = y;
    pt.y = x;
    GetWindowRect(GameWindow.hwnd, &rect);

    SetCursorPos(rect.left + pt.x, rect.top + pt.y);

    SetForegroundWindow(GameWindow.hwnd);
    MOUSEINPUT mDown;
    mDown.mouseData = 0;
    mDown.time = 0;
    mDown.dwExtraInfo = 0;
    mDown.dy = 0;
    mDown.dx = 0;
    mDown.dwFlags = MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

    INPUT inputMouseDown, inputMouseUp, inputMouseMove;
    inputMouseDown.type = INPUT_MOUSE;
    inputMouseDown.mi = mDown;

    INPUT event[1] = {inputMouseDown};
    SendInput(1, event, sizeof(INPUT));
}

