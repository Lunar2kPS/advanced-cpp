#include "Skill.h"
#include <math.h>

namespace carlos {
    int Skill::getEXPForNextLevel(int currentLevel) {
        if (currentLevel < MIN_LEVEL || currentLevel >= MAX_LEVEL)
            return 0;
        return 4 * (int) floorf(powf(currentLevel + 1, 2.4f)) + 35;
    }

    void Skill::gainEXP(int amount) {
        if (amount <= 0)
            return;
        if (level >= MAX_LEVEL)
            return;

        int sum = exp + amount;
        int expForNext = getEXPForNextLevel();
        
        while (sum >= expForNext) {
            level++;
            sum -= expForNext;
            expForNext = getEXPForNextLevel();
        }
        exp = sum;
    }

    ostream& operator <<(ostream& output, const Skill& skill) {
        if (!skill.name.empty())
            output << skill.name << " ";

        output << "Lv. " << skill.level << " (EXP: " << skill.exp;
        if (skill.level >= Skill::MIN_LEVEL && skill.level < Skill::MAX_LEVEL)
            output << " / " << skill.getEXPForNextLevel();
        output << ")";
        return output;
    }
}
