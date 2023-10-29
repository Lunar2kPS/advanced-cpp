#pragma once

#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class TimeSystem : public IGameLoopSystem {
        private:
            double appTime;
            float dt;

        public:
            double timeAsDouble();
            float time();
            float deltaTime();

            TimeSystem() { }

            int getOrder() { return -9999; } //NOTE: Depends on WindowSystem for GLFW time
            void earlyUpdate();
            void update() { }
            void render() { }
    };
}
