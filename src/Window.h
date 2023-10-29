#pragma once

#include "GraphicsAPI.h"

namespace carlos {
    class Window {
        private:
            void* windowPtr;
            GraphicsAPI graphicsAPI;
            int width;
            int height;

        public:
            Window(void* windowPtr, GraphicsAPI graphicsAPI, int width, int height) :
                windowPtr(windowPtr),
                graphicsAPI(graphicsAPI),
                width(width),
                height(height) { }

            void* getPtr();
            GraphicsAPI getGraphicsAPI();
            int getWidth();
            int getHeight();

            void updateSize(int width, int height);
    };
}
