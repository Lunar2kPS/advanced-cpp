#pragma once

#include <vector>
#include "entt/entt.hpp"

#include "components/Component.h"
#include "interfaces/IGameLoopSystem.h"

using namespace entt;
using std::vector;

namespace carlos {
    class SceneSystem {
        private:
            registry entityRegistry;
            vector<GameObject*> gameObjects;
        public:
            SceneSystem();
            ~SceneSystem();

            int getOrder() { return -1000; }
    };
}
