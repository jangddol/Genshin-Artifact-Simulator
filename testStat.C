#include "top.hh"


void testStat()
{
    Stat stat = Stat();
    stat.SetZero();
    stat.Update();
    PrintStat(stat);
}