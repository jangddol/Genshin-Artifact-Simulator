#include <vector>


using namespace std;


class tempArray
{
public:
    tempArray()
    {
        mArray = new double[10];
        SetZero();
    }
    ~tempArray()
    {
        delete mArray;
    }

    void   SetZero() { for (int i = 0; i < 10; i++) mArray[i] = 0.; }
    double GetElement(int index) { return mArray[index]; }
    void   SetElement(int index, double amount) { mArray[index] = amount; }

private:
    double* mArray;
};


void test()
{
    tempArray* temp = new tempArray();
    for (int i = 0; i < 10; i++)
    {
        cout << temp->GetElement(i) << endl;
    }
    delete temp;
    for (int i = 0; i < 10; i++)
    {
        cout << temp->GetElement(i) << endl;
    }
}