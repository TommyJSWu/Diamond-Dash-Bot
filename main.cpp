#include <stdio.h>
#include "util.h"
#include "DiamondDashBot.h"

bool checkEndGame(int nMoves, int lastnMoves, Move* moves, Move* lastmoves)
{
    int i;
    if(!nMoves || !lastnMoves || nMoves != lastnMoves) return false;
    //if(nMoves > 10) return true;
    for(i=0;i<lastnMoves;i++)
        if(moves[i].x != lastmoves[i].x || moves[i].y != lastmoves[i].y) return false;
    puts("tru");
    return true;
}

main()
{
    unsigned **rawPixel;
    DIAMOND **board;
    int i, j, nMoves, lastnMoves; 

    while(!FindGameWindow())
    {
        puts("Not found!");
        Sleep(250);
    }
    puts("Found!");
    rawPixel = new unsigned*[BoardH * GridH];
    for(i=0;i<BoardH*GridH;i++)
        rawPixel[i] = new unsigned[BoardW * GridW];
    board = new DIAMOND*[BoardH * BoardW];
    for(i=0;i<BoardH;i++)
        board[i] = new DIAMOND[BoardW];
    
    Move *moves = NULL, *lastmoves = NULL;
    nMoves = lastnMoves = 0;
    const int tryBound = 20;
    while(true)
    {
        delete [] lastmoves;
        lastmoves = moves;
        lastnMoves = nMoves;
        int tryCount = 0, err;
        while((err=GetBoard(board, rawPixel)) > 30 && tryCount < tryBound)
        {
            printf("try: %d err: %d\n", tryCount, err);
            Sleep(20);
            tryCount++;
        }
        //puts("After get raw board");
        if(tryCount >= tryBound){ puts("more than 5"); break;}
        //puts("After processing raw board");
        moves = GetMoves(board, nMoves);
        if(checkEndGame(nMoves, lastnMoves, moves, lastmoves)) break;
        
        /*
        for(i=0;i<BoardH;i++)
        {
            for(j=0;j<BoardW;j++)
                printf("%d ", board[i][j]);
            puts("");
        }
        */
        for(i=0;i<nMoves;i++)
        {
            if(GetBoard(board, rawPixel) >= 60) break;
            /*
            if(board[moves[i].x][moves[i].y] == moves[i].diamond)
            {
                printf("%d %d\n", moves[i].x, moves[i].y);
                system("pause");
            }
            */
            if(board[moves[i].x][moves[i].y] == moves[i].diamond)
            {
                ExecMove(moves[i].x, moves[i].y);
                Sleep(120);
                GetBoard(board, rawPixel);
                if(board[moves[i].x][moves[i].y] == SUPER)
                {
                    Sleep(1360);
                    i = nMoves;
                    break;
                }
            }
            else
            {
                //i = nMoves;
                //break;
            }
                //Sleep(600);
            //else Sleep(100);
            //Sleep(100);
        }
        if(i<nMoves) break;
        Sleep(150);
    }

    for(i=0;i<BoardH*GridH;i++)
        delete[] rawPixel[i];
    delete [] rawPixel;
    for(i=0;i<BoardH;i++)
        delete[] board[i];
    delete[] board;
}
