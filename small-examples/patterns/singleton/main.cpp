#include <iostream>

#include "Singleton.h"

using std::cout;
using std::endl;

int main() {
    cout << "Programming Design Pattern: Singleton" << endl;
    Singleton* instance = Singleton::getInstance();

    //Example of a 2nd call, only uses 1 instance still:
    Singleton* same = Singleton::getInstance();

    cout << "Same instance? " << (instance == same ? "true" : "false") << endl;

    Singleton::destroyInstance();
    return 0;
}
