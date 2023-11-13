#pragma once

#include <vector>

#include "interfaces/IWindowSystem.h"

using std::vector;

namespace carlos {
    class WindowSystem : public IWindowSystem {
        private:
            bool glfwInitialized = false;
            vector<Window*> windows = { };

        public:
            WindowSystem();
            ~WindowSystem();

            bool tryCreateWindow(const char* title, int width, int height, Window*& outWindow);
            void setTitle(Window& window, const char* title);
            Window* getMainWindow();
            bool anyWindowOpen();

            int getOrder() { return -10000; }

            void earlyUpdate();
            void update();
            void render();
            void postRender();
    };
}
