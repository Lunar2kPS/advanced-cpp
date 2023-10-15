#include <iostream>

#include "ServiceLocator.h"

using std::cout;
using std::endl;

struct Example {
    int intValue;
};

struct Other {
    bool boolValue;
};

int main() {
    ServiceLocator locator = ServiceLocator();
    Example* e = new Example();
    e->intValue = 44;
    locator.addSystem(e);

    Example* example = locator.getSystem<Example>();
    Other* other = locator.getSystem<Other>();

    cout << "Example* found: " << (example != nullptr) << endl;
    cout << "Other* found: " << (other != nullptr) << endl;

    if (example != nullptr)
        cout << "example's intValue = " << example->intValue << endl;
    if (other != nullptr)
        cout << "other's boolValue = " << other->boolValue << endl;

    return 0;
}
