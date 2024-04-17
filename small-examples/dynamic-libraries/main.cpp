#include <iostream>
#include "library/Player.h"

using std::cout;
using std::endl;

using namespace carlos;

int main() {
    cout << "(Small) main program!" << endl;
    Player me = Player("ModLunar");
    cout << "me.getName() = " << me.getName() << endl;
    return 0;
}
