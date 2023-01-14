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