#include <iostream>
#include <sstream>

#include "Skill.h"

using std::cout;
using std::cerr;
using std::endl;
using std::stringstream;
using std::string;

using namespace carlos;

int main() {
    Skill attack = Skill("Attack");
    if (attack.getLevel() != Skill::MIN_LEVEL) {
        cerr << "[ERROR] Skill did not start at the min level." << endl;
        return 1;
    }
    cout << "[INFO] SUCCESS! Skill starts at min level." << endl;


    {
        stringstream ss = { };
        ss << attack;
        string result = ss.str();
        string expected = "Attack Lv. 1 (EXP: 0 / 55)";
        int comparison = expected.compare(result);
        if (comparison != 0) {
            cerr << "[ERROR] Unexpected string concatenation result: " << result
            << "\n            Was expecting instead: " << expected << endl;
            return 2;
        }
        cout << "[INFO] SUCCESS! Named skill concatenates properly." << endl;
    }

    {
        Skill unnamed = Skill("");

        stringstream ss = { };
        ss << unnamed;
        string result = ss.str();
        string expected = "Lv. 1 (EXP: 0 / 55)";
        int comparison = expected.compare(result);
        if (comparison != 0) {
            cerr << "[ERROR] Unexpected string concatenation result: " << result
            << "\n            Was expecting instead: " << expected << endl;
            return 3;
        }
        cout << "[INFO] SUCCESS! Unnamed skill concatenates properly." << endl;
    }

    attack.gainEXP(15);
    if (attack.getEXP() != 15) {
        cerr << "[ERROR] Unexpected EXP after gaining 15: " << attack << endl;
        return 4;
    }
    cout << "[INFO] SUCCESS! Skill gains EXP (no level-up) properly." << endl;

    attack.gainEXP(40);
    if (attack.getLevel() != Skill::MIN_LEVEL + 1 || attack.getEXP() != 0) {
        cerr << "[ERROR] Unexpected level and/or EXP after gaining exactly the amount needed for 1 level-up! " << attack << endl;
        return 5;
    }
    cout << "[INFO] SUCCESS! Skill gains EXP for 1 level-up properly." << endl;

    Skill magic = Skill("Magic");
    magic.gainEXP(55 + 87);
    if (magic.getLevel() != Skill::MIN_LEVEL + 2 || magic.getEXP() != 0) {
        cerr << "[ERROR] Unexpected level and/or EXP after gaining exactly the amount needed for 2 level-ups! " << magic << endl;
        return 6;
    }
    cout << "[INFO] SUCCESS! Skill gains EXP for 2 level-ups properly." << endl;

    Skill archery = Skill("Archery");
    archery.gainEXP(55 + 100);
    if (archery.getLevel() != Skill::MIN_LEVEL + 2 || archery.getEXP() != 13) {
        cerr << "[ERROR] Unexpected level and/or EXP after gaining a bit more than 2 level-up's worth of EXP! " << archery << endl;
        return 7;
    }
    cout << "[INFO] SUCCESS! Skill gains EXP for a bit more than 2 level-ups properly." << endl;

    return 0;
}
