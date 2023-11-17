#include "systems/CameraSystem.h"

#include "interfaces/IWindowSystem.h"
#include "ServiceLocator.h"

namespace carlos {
    void CameraSystem::render() {
        IWindowSystem* windowing = DefaultServiceLocator::getSystem<IWindowSystem>();
        Window* mainWindow = nullptr;
        if (windowing != nullptr) {
            mainWindow = windowing->getMainWindow();
            if (mainWindow != nullptr) {
                int w = mainWindow->getWidth();
                int h = mainWindow->getHeight();

                if (w != previousWidth || h != previousHeight) {
                    previousWidth = w;
                    previousHeight = h;
                    proj = glm::ortho<float>(0, w / pixelScale, 0, h / pixelScale, -1, 1);
                }
            }
        }
    }
}
