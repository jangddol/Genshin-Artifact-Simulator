#include <vector>


using namespace std;

void test()
{
    vector<double> tempVec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    int count = 0;
    for (int i = 0; i < tempVec.size(); i++)
    {
        if (tempVec.at(i) == 4)
        {
            tempVec.erase(tempVec.begin() + i);
            i--;
        }
        cout << tempVec.size() << endl;
    }
    
    for (int i = 0; i < tempVec.size(); i++)
    {
        cout << tempVec.at(i) << endl;
    }
}