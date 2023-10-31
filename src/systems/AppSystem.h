#pragma once

#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class AppSystem : public IGameLoopSystem {
        private:
            bool quitFlag;

        public:
            bool isQuitRequested() { return quitFlag; }
            bool requestQuit() {
                quitFlag = true;
                return true;
            }
    };
}
