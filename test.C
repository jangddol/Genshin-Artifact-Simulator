class Parent
{
public:
    Parent(){}
    ~Parent(){}

    void PrintTemp(){ cout << mTemp << endl; }

protected:
    int mTemp = 0;

};

class Child : public Parent
{
public:
    Child(){ mTemp = 1; }
    ~Child(){}
};

void test()
{
    Child tempChild = Child();
    tempChild.PrintTemp();
    Parent tempParent = *(Parent*)&tempChild;
    tempParent.PrintTemp();
}