#pragma once

#include "imgui.h"
#include "interfaces/IGameLoopSystem.h"

namespace carlos {
    class GUI : public IGameLoopSystem {
        private:
            bool initialized = false;
            char* nameBuffer;
            ImFont* customFont;
            // ImVec2 windowPos;
            // ImVec2 windowSize;
            // bool* isOpen;
            void onGUI();

        public:
            GUI();
            ~GUI();

            int getOrder() { return 10000; }
            void update();
            void render();
    };
}