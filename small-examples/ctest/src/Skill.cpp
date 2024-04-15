#include "Skill.h"
#include <math.h>

namespace carlos {
    int Skill::getEXPForNextLevel(int currentLevel) {
        if (currentLevel < MIN_LEVEL || currentLevel >= MAX_LEVEL)
            return 0;
        return 4 * (int) floorf(powf(currentLevel + 1, 2.4f)) + 35;
    }

    ostream& operator<<(ostream& output, const Skill& skill) {
        output << skill.name << ": LVL " << skill.level << ", EXP: " << skill.exp;
        if (skill.level >= Skill::MIN_LEVEL && skill.level < Skill::MAX_LEVEL)
            output << " / " << skill.getEXPForNextLevel();
        return output;
    }
}
