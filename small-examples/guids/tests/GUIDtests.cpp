#include <iostream>
#include "GUID.h"

using std::cout;
using std::endl;

using namespace carlos;

int main() {
    GUID empty = GUID();
    cout << empty << endl;

    GUID unique = GUID::createNew();
    cout << unique << endl;
    return 0;
}
