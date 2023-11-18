#pragma once

#include <vector>

#include "components/Component.h"
#include "interfaces/IGameLoopSystem.h"
#include "GameObject.h"

using std::vector;

namespace carlos {
    /// @brief Contains a list of GameObjects.
    /// @note This can be saved to a JSON scene file.
    class SceneSystem : public IGameLoopSystem {
        private:
            vector<GameObject*> gameObjects;
        public:
            SceneSystem();
            ~SceneSystem();

            /// @brief Adds an existing GameObject to the scene system. This takes ownership over deleting the GameObject when this SceneSystem is deleted.
            /// @note TODO: Create Scene objects and move this reponsibility to the individual scenes instead.
            /// @param gameObject The GameObject to add.
            void add(GameObject* gameObject) { gameObjects.push_back(gameObject); } //TODO: Null-check

            int getOrder() { return -1000; }

            void update();
            void render();
    };
}
