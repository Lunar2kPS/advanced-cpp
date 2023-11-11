#pragma once

#include "interfaces/IGameLoopSystem.h"
#include "Window.h"

namespace carlos {
    /// @brief Represents a system that can open windows on the OS for the currently running application.
    class IWindowSystem : public IGameLoopSystem {
        public:
            virtual bool tryCreateWindow(const char* title, int width, int height, Window*& outWindow) = 0;
            virtual Window* getMainWindow() = 0;
            virtual bool anyWindowOpen() = 0;
    };
}
