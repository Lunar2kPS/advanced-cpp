#include "systems/TimeSystem.h"

#include <iostream>

using std::cout;
using std::endl;

#include "openglutility.h"

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
