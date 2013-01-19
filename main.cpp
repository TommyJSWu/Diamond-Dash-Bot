#include <cstdio>

#include "util.h"


main()
{
    while(!FindGameWindow())
    {
        puts("Not found!");
        Sleep(250);
    }
    puts("Found!");
    scanf(" ");
}
