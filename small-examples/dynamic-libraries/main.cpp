#include <iostream>
#include "library/SomeClass.h"

using std::cout;
using std::endl;

using namespace examplelibrary;

int main() {
    cout << "(Small) main program!" << endl;
    SomeClass c = SomeClass("Test");
    cout << "c.getName() = " << c.getName() << endl;
    return 0;
}
