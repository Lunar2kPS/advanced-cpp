#include <iostream>
#include <memory>

#include "Singleton.h"
#include "SmartSingleton.h"

using std::cout;
using std::endl;
using std::shared_ptr;

int main() {
    cout << "Programming Design Pattern: Singleton" << endl;
    Singleton* instance = Singleton::getInstance();
    Singleton* same = Singleton::getInstance(); //Example of a 2nd call, only uses 1 instance still

    cout << "Same instance? " << (instance == same ? "true" : "false") << endl;
    Singleton::destroyInstance();

    cout << " --- --- --- " << endl;

    shared_ptr<SmartSingleton> smartInstance = SmartSingleton::getInstance();
    shared_ptr<SmartSingleton> sameSmart = SmartSingleton::getInstance();
    cout << "Same instance? " << (smartInstance == sameSmart ? "true" : "false") << endl;

    return 0;
}
