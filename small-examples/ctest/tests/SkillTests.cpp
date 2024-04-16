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

    return 0;
}
