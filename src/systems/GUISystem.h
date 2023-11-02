#pragma once

#include "imgui.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class GUISystem : public IGameLoopSystem {
        private:
            bool initialized = false;
            char* nameBuffer;
            ImFont* customFont;
            // ImVec2 windowPos;
            // ImVec2 windowSize;
            // bool* isOpen;
            void onGUI();

        public:
            GUISystem();
            ~GUISystem();

            int getOrder() { return 10000; }
            void update();
            void render();
    };
}