#pragma once

#include <iostream>
#include <string>

using std::string;
using std::ostream;

namespace carlos {
    /// @brief Represents expertise in a certain area, that can be improved as a character continues to practice it.
    class Skill {
        public:
            static const int MIN_LEVEL = 1;
            static const int MAX_LEVEL = 99;
            static int getEXPForNextLevel(int currentLevel);
        private:
            string name = "";
            int level = MIN_LEVEL;
            int exp = 0;

        public:
            Skill(string name) : name(name) { }
            const string& getName() const { return name; }
            int getLevel() const { return level; }
            int getEXP() const { return exp; }
            int getEXPForNextLevel() const { return getEXPForNextLevel(level); }
            int getRemainingEXP() const {
                int expForNext = getEXPForNextLevel();
                if (expForNext <= 0)
                    return 0;
                return expForNext - exp;
            }
            void gainEXP(int exp) {
                if (exp <= 0)
                    return;
                if (level >= MAX_LEVEL)
                    return;

                int sum = this->exp + exp;
                int expForNext = getEXPForNextLevel();
                if (sum < expForNext) {
                    this->exp = sum;
                    return;
                }

                //TODO: Fix case where you gain enough EXP for multiple-level-ups!
                level++;
                this->exp = sum - expForNext;
            }

            friend ostream& operator <<(ostream& output, const Skill& skill);
    };
}
