#include "SmartSingleton.h"

#include <iostream>

using std::cout;
using std::endl;
using std::make_shared;

//NOTE: I use a weak_ptr here because I DON'T want this to count towards the ref count!
//If the USER of this singleton class has completely finished using this class, like the main program is ending,
//Then this singleton's destructor should be called!
weak_ptr<SmartSingleton> SmartSingleton::instance;

shared_ptr<SmartSingleton> SmartSingleton::getInstance() {
    shared_ptr<SmartSingleton> result = nullptr;

    if (instance.expired()) {
        //NOTE: I don't want to just make a T() because then it goes out of scope and the destructor is called.

        //WARNING: make_shared<T>(...) was just generating compiler errors when I tried to pass in a T*
        //      But it works when passing in a T.
        //THIS DOESN'T WORK:
        // result = make_shared<SmartSingleton>(new SmartSingleton());
        
        //THIS WORKS:
        //      So instead, I make a new T() and just dereference that:
        // SmartSingleton* rawPtr = new SmartSingleton();
        // result = make_shared<SmartSingleton>(*rawPtr);
        
        //THIS WORKS:
        //      But then I realized oh, I was using the make_shared<T>(T) function.
        //      I can also use the shared_ptr<T>(T*) constructor:
        // result = shared_ptr<SmartSingleton>(new SmartSingleton());

        //THIS WORKS (only iff there's a public constructor that matches in parameters):
        //      I read that it's preferred to use make_shared<T>(...), and it's a variadic function, so it just passes the parameters to the constructor of your type.
        //      But oh wait, but this is a Singleton class. This depends on a PUBLIC constructor, haha... oops.
        // result = make_shared<SmartSingleton>();

        
        //Okay so we'll create the Singleton like this then:
        SmartSingleton* raw = new SmartSingleton();
        result = shared_ptr<SmartSingleton>(raw);

        instance = result;
    } else {
        //This creates 1 more shared_ptr<T> that points to our singleton instance.
        result = instance.lock();
    }

    return result;
}

SmartSingleton::SmartSingleton() {
    cout << "(Constructor on smart version)" << endl;
}

SmartSingleton::~SmartSingleton() {
    cout << "(Destructor on smart version)" << endl;
}
