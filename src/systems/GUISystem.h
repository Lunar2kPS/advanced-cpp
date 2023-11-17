#pragma once

#include "imgui.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    /// @brief Represents a system that would render the editor GUI of the game engine.
    /// Currently, this just draws the Dear ImGUI demo window.
    class GUISystem : public IGameLoopSystem {
        private:
            bool initialized = false;
            char* nameBuffer;
            ImFont* customFont;
            void onGUI();

        public:
            GUISystem();
            ~GUISystem();

            int getOrder() { return 1000; }
            void update();
            void render();
    };
}