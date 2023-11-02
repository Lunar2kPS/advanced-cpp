#include "MeshRenderer.h"

#include "glmutility.h"
#include "ServiceLocator.h"
#include "interfaces/IWindowSystem.h"
#include "GameObject.h"
#include "components/Transform.h"

namespace carlos {
    void MeshRenderer::render() {
        if (mesh == nullptr || material == nullptr)
            return;

        Transform* transform = getGameObject()->getComponent<Transform>();

        //TODO: Put this orthographic projection code into a main screen/rendering system..
        IWindowSystem* windowing = DefaultServiceLocator::getSystem<IWindowSystem>();
        Window* mainWindow = nullptr;
        if (windowing != nullptr) {
            mainWindow = windowing->getMainWindow();
            if (mainWindow != nullptr) {
                //NOTE: mat4() is all zeroes.
                //      mat4(1) is the identity matrix, which is equivalent to:
                //      mat4(
                //          1, 0, 0, 0,
                //          0, 1, 0, 0,
                //          0, 0, 1, 0,
                //          0, 0, 0, 1
                //      )
                
                int pxScale = 4;
                mat4 proj = glm::ortho<float>(0, mainWindow->getWidth() / pxScale, 0, mainWindow->getHeight() / pxScale, -1, 1);
                mat4 view = mat4(1);
                mat4 model = (transform != nullptr) ? glm::translate(mat4(1), vec3(transform->getLocalPosition(), 0)) : mat4(1);
                
                //NOTE: The multiplication appears as P V M because GLM adheres to column-major ordering in memory layout,
                //  as opposed to row-major ordering. OpenGL uses column-major, so when setting uniforms,
                //  because GLM adheres to the same conventions as OpenGL, no transposition is necessary.
                mat4 mvp = proj * view * model;
                material->setMatrix("mvp", mvp);
            }
        }
        
        material->bind();
        mesh->bind();
        mesh->renderDirect();
        // mesh->unbind();
    }
}
