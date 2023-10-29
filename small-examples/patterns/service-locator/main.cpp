#include <iostream>

#include "ServiceLocator.h"

using std::cout;
using std::endl;

class IExample {
    public:
        virtual int getInt() = 0;
};

class Example : public IExample {
    public:
        int intValue;
        int getInt() { return intValue; }
};

class Other {
    public:
        bool boolValue;
};

int main() {
    ServiceLocator locator = ServiceLocator();
    Example* e = new Example();
    e->intValue = 44;
    locator.addSystem<IExample>(e);

    IExample* example = locator.getSystem<IExample>();
    Other* other = locator.getSystem<Other>();

    cout << "IExample* found: " << (example != nullptr) << endl;
    cout << "Other* found: " << (other != nullptr) << endl;

    if (example != nullptr)
        cout << "example's intValue = " << example->getInt() << endl;
    if (other != nullptr)
        cout << "other's boolValue = " << other->boolValue << endl;

    return 0;
}
