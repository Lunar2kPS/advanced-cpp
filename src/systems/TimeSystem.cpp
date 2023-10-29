#include "systems/TimeSystem.h"

#include <iostream>

using std::cout;
using std::endl;

//WARNING: GLFW must be included AFTER OpenGL
// #include "openglutility.h"
#include "GLFW/glfw3.h"

namespace carlos {
    double TimeSystem::timeAsDouble() { return appTime; }
    float TimeSystem::time() { return appTime; }
    float TimeSystem::deltaTime() { return dt; }

    void TimeSystem::earlyUpdate() {
        double prevTime = appTime;
        appTime = glfwGetTime();
        dt = appTime - prevTime;
    }
}
