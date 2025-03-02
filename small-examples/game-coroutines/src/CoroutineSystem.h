#pragma once

#include <vector>
#include "Coroutine.h"

using std::vector;

namespace carlos {
    class CoroutineSystem {
        private:
            vector<Coroutine*> coroutines = { };

        public:
            void startCoroutine(function<void(Coroutine&)> func) {
                coroutines.push_back(new Coroutine(func));
            }

            void update(float dt) {
                for (vector<Coroutine*>::iterator i = coroutines.begin(); i != coroutines.end(); ) {
                    if ((*i)->getIsCompleted()) {
                        i = coroutines.erase(i);
                    } else {
                        (*i)->update(dt);
                        i++;
                    }
                }
            }
    };
}
