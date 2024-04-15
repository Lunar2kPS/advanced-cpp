#include <iostream>
#include "Skill.h"

using std::cout;
using std::endl;

using namespace carlos;

int main() {
    cout << "Main Program!" << endl;

    Skill attack = Skill("Attack");
    Skill magic = Skill("Magic");

    attack.gainEXP(60);

    cout << attack << "\n";
    cout << magic << "\n";

    return 0;
}
