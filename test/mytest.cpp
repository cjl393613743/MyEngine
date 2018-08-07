#include <iostream>
using namespace std;

class Obj
{
    public:
        int *p;
};

int*& func(Obj *p)
{
    return p->p;
}

int main()
{
    int i = 1;
    int i2 = 2;
    Obj obj;
    obj.p = &i;

    int *tmp = func(&obj);
    tmp = &i2;
    cout<<*(obj.p)<<endl;

    return 0;
}
