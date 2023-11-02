#include "Window.h"

namespace carlos {
    void* Window::getPtr() { return windowPtr; }
    GraphicsAPI Window::getGraphicsAPI() { return graphicsAPI; }
    int Window::getWidth() { return width; }
    int Window::getHeight() { return height; }
    void Window::updateSize(int width, int height) {
        this->width = width;
        this->height = height;
    }
}
