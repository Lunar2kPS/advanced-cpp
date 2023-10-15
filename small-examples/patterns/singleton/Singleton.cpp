#include "Singleton.h"

#include <iostream>
#include <fstream>

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
    std::cout << "(Constructor)" << std::endl;
}

Singleton::~Singleton() {
    std::cout << "(Destructor)" << std::endl;
}
