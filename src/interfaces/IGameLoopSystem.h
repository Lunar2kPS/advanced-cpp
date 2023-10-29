#pragma once

#include "interfaces/ISystem.h"

namespace carlos {
    class IGameLoopSystem : public ISystem {
        public:
            IGameLoopSystem() { }
            ~IGameLoopSystem() { }

            virtual void earlyUpdate() { }
            virtual void update() = 0;
            virtual void render() = 0;
            virtual void postRender() { }
    };
}
