#include "Singleton.h"

#include <iostream>

using std::cout;
using std::endl;

Singleton* Singleton::instance;

Singleton* Singleton::getInstance() {
    if (instance == nullptr)
        instance = new Singleton();
    return instance;
}

bool Singleton::destroyInstance() {
    if (instance == nullptr)
        return false;
    delete instance;
    return true;
}

Singleton::Singleton() {
    cout << "(Constructor)" << endl;
}

Singleton::~Singleton() {
    cout << "(Destructor)" << endl;
}
