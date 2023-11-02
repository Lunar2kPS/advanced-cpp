#include "systems/SceneSystem.h"

#include <iostream>

#include "components/MeshRenderer.h"

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
        for (GameObject* g : gameObjects) {
            MeshRenderer* renderer = g->getComponent<MeshRenderer>();
            if (renderer != nullptr)
                renderer->render();
        }
    }
}
