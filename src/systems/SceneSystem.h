#pragma once

#include <vector>

#include "components/Component.h"
#include "interfaces/IGameLoopSystem.h"
#include "GameObject.h"

using std::vector;

namespace carlos {
    class SceneSystem : public IGameLoopSystem {
        private:
            vector<GameObject*> gameObjects;
        public:
            SceneSystem();
            ~SceneSystem();
            void add(GameObject* gameObject) { gameObjects.push_back(gameObject); } //TODO: Null-check

            int getOrder() { return -1000; }

            void update();
            void render();
    };
}
