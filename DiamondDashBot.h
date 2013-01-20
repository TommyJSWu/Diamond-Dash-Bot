#ifndef DIAMONDDASHBOT
#define DIAMONDDASHBOT

enum DIAMOND
{
    RED = 0,
    GREEN,
    YELLOW,
    PURPLE,
    BLUE,
    SUPER,
    ERRORD
};

struct Move
{
    char x, y;
};

const int BoardW = 10, BoardH = 9, MarginY = 93, MarginX = 79;

#endif
