#pragma once

#include "interfaces/Key.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class IInputSystem : public IGameLoopSystem {
        public:
            int getOrder() { return -9900; }

            virtual bool getKey(Key key) = 0;
            virtual bool getKeyDown(Key key) = 0;
            virtual bool getKeyUp(Key key) = 0;
    };
}
