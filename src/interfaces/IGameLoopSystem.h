#pragma once

#include "interfaces/ISystem.h"

namespace carlos {
    /// @brief An interface of a system that can run during the program's game loop.
    class IGameLoopSystem : public ISystem {
        public:
            //I guess this always has "virtual" behavior by default, so we can't mark constructors virtual:
            IGameLoopSystem() { }
            
            //NOTE: This is important to be VIRTUAL, otherwise we'd get undefined behavior with "delete" calls
            //  on IGameLoopSystem* pointers, and child/subclasses' destructors won't be called polymorphically!
            virtual ~IGameLoopSystem() { }

            virtual void earlyUpdate() { }
            virtual void update() { }
            virtual void render() { }
            virtual void postRender() { }
    };
}
