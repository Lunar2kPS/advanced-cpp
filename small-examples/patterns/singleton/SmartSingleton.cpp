#include "SmartSingleton.h"

#include <iostream>

using std::cout;
using std::endl;
using std::make_shared;

SmartSingleton SmartSingleton::instance = SmartSingleton();

shared_ptr<SmartSingleton> SmartSingleton::getInstance() {
    return make_shared<SmartSingleton>(instance);
}

SmartSingleton::SmartSingleton() {
    cout << "(Constructor on smart version)" << endl;
}

SmartSingleton::~SmartSingleton() {
    cout << "(Destructor on smart version)" << endl;
}
