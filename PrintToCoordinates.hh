#pragma once

#include <iostream>
#include <string>
#include <stdarg.h>
#include <mutex>


constexpr static int FIRSTCOLUMNWIDTH = 18;
constexpr static int COLUMNWIDTH = 9;

std::mutex m1;
void printToCoordinates(int y, int x, const char* format, ...)
{
    std::lock_guard<std::mutex> lock_guard(m1);
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", y, x);
    vprintf(format, args);
    va_end(args);
    fflush(stdout);
}