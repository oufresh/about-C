#include <iostream>
#include <vector>
#include <string>
#include "func.h"

using namespace std;

int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;

    cout << "test myFunc" << endl;
    int ret = Func::mySum(10, 100);
    cout << "Result " << ret << endl;
}