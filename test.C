#include <vector>
#include <array>
#include "top.hh"


using namespace std;


class tempArray
{
public:
    tempArray()
    {
        mArray = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    }
    ~tempArray()
    {
    }

    void   SetZero() { for (int i = 0; i < 10; i++) mArray[i] = 0.; }
    double GetElement(int index) { return mArray[index]; }
    void   SetElement(int index, double amount) { mArray[index] = amount; }

private:
    std::array<double, 10> mArray;
};


void test()
{
    int N = 10000000;
    for (int j = 0; j < N; j++)
    {
        Stat* temp = new Stat();
        for (int i = 0; i < 10; i++)
        {
            PrintStat(*temp);
        }
        delete temp;
        for (int i = 0; i < 10; i++)
        {
            PrintStat(*temp);
        }
    }
}