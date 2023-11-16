#pragma once

#include "GraphicsAPI.h"

namespace carlos {
    /// @brief Represents one of the window(s) used for the current application.
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

            /// @brief Gets the underlying native pointer of this window used by the windowing system
            /// to perform operations on the window, such as setting the title, changing the width and height, etc.
            void* getPtr();

            /// @brief Gets the graphics API used for the current window, such as OpenGL or OpenGL ES.
            GraphicsAPI getGraphicsAPI();

            /// @brief Gets the width in pixels of the window.
            int getWidth();

            /// @brief Gets the height in pixels of the window.
            int getHeight();

            void updateSize(int width, int height);
    };
}
