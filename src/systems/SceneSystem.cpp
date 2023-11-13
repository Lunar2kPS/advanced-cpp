#include "systems/SceneSystem.h"

#include <iostream>

#include "components/MeshRenderer.h"
#include "components/Transform.h"
#include "systems/CameraSystem.h"
#include "ServiceLocator.h"

using std::cout;
using std::endl;

namespace carlos {
    SceneSystem::SceneSystem() {

    }

    SceneSystem::~SceneSystem() {
        for (int i = gameObjects.size() - 1; i >= 0; i--)
            delete gameObjects[i];
    }

    void SceneSystem::update() {
        // for (GameObject* g : gameObjects) {
        //     vector<Component*>& components = g->getAllComponents();
        //     for (int i = 0; i < components.size(); i++)
        //         components[i]->update();
        // }
    }

    void SceneSystem::render() {
        CameraSystem* cameras = DefaultServiceLocator::getSystem<CameraSystem>();

        for (GameObject* g : gameObjects) {
            MeshRenderer* renderer = g->getComponent<MeshRenderer>();
            if (renderer != nullptr) {
                Material* material = renderer->getMaterial();
                if (material != nullptr) {
                    Transform* transform = g->getComponent<Transform>();

                    mat4 model = (transform != nullptr) ? glm::translate(mat4(1), vec3(transform->getLocalPosition(), 0)) : mat4(1);
                    mat4 mvp = cameras->getMVP(model);
                    material->setMatrix("mvp", mvp);
                }
                renderer->render();
            }
        }
    }
}
