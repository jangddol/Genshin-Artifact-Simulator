#include <iostream>
#include <string>
#include <stdarg.h>


void printToCoordinates(int y, int x, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", y, x);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}


int terminalOutputTest()
{
#ifdef _MSC_VER
    system("cls");
#else
    system("clear");
#endif

    for ( int i=0; i<1000000; i++)
    {
        printToCoordinates(5, 10, "a1 %d", i);
        printToCoordinates(5, 20, "a2 %d", i);
        printToCoordinates(5, 30, "a3 %d", i);

    }

    return 0;

}