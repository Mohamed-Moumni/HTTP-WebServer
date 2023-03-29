#include <iostream>

using namespace std;

class A
{
    public:
        void func(void)
        {
            static int a = 0;
            a++;
            cout << a << endl;
        }
};

int main(int ac, char *av[])
{
    A first;
    A second;

    first.func();
    first.func();
    first.func();
    second.func();
    second.func();
    second.func();
    return (0);
}