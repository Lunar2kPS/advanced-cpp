#include <iostream>
#include <sstream>

#include "glad/gl.h"
#include "GLFW/glfw3.h"

using std::cout;
using std::endl;
using std::stringstream;

void glfwErrorCallback(int errorCode, const char* description);

int main(int argCount, char** args) {
    GLFWwindow* window = nullptr;
    int width = 800;
    int height = 600;
    const char* title = "OpenGL Example"; //TODO: Use CMake to transfer the project(...) title in here. We could use a separate .h file too.

    if (!glfwInit()) {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_OPENGL_PROFILE,             GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,      GL_TRUE);

    glfwSetErrorCallback(glfwErrorCallback);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        return 3;
    }
    printf("Loaded OpenGL%d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    //NOTE: V-Sync: Wait 1 frame before rendering each frame --
    //      don't waste CPU resources trying to render at 2000 FPS when our screens can't even display that fast!
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glfwGetWindowSize(window, &width, &height);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    if (window != nullptr)
        glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void glfwErrorCallback(int errorCode, const char* description) {
    stringstream ss;
    ss << "GLFW Error code: " << errorCode << "\n";
    ss << description;
    ss << "\n";

    const char* message = ss.str().c_str();
    fprintf(stderr, "%s", message);
}
