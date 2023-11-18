#pragma once

#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    /// @brief A system that keeps track of the app time and delta time (often abbreviated as dt).
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
