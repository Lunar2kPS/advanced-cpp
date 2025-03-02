#pragma once

#include <functional>

using std::function;

#include <iostream>
using std::cout;
using std::endl;

namespace carlos {
    class Coroutine;
    typedef void (*CoroutineCallback)(Coroutine& self, void* userData);

    class Coroutine {
        private:
            function<void(Coroutine&)> func = nullptr;
            CoroutineCallback nextStep = nullptr;
            void* nextUserData = nullptr;

            bool isStarted = false;
            bool isCompleted = false;
            float localTime = 0;
            float nextStepTime = 0;

        public:
            Coroutine(function<void(Coroutine&)> func) : func(func) { }

            bool getIsStarted() { return isStarted; }
            bool getIsCompleted() { return isCompleted; }
            void update(float dt) {
                if (isCompleted)
                    return;

                localTime += dt;
                if (!isStarted) {
                    isStarted = true;
                    func(*this);
                    if (nextStep == nullptr) {
                        complete();
                        return;
                    }
                } else if (localTime >= nextStepTime) {
                    CoroutineCallback previous = nextStep;
                    nextStep(*this, nextUserData);
                    if (nextStep == previous) {
                        complete();
                        return;
                    }
                }
            }

            void waitForSeconds(float seconds, void* userData, CoroutineCallback nextStep) {
                nextStepTime += seconds;
                this->nextStep = nextStep;
                nextUserData = userData;
                cout << "nextStep => " << nextStep << endl;
            }

            void complete() {
                cout << "complete!" << endl;
                isCompleted = true;
            }
    };
}
