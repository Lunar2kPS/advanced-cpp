#pragma once

#include <vector>
#include "entt/entt.hpp"

#include "components/Component.h"
#include "interfaces/IGameLoopSystem.h"
#include "GameObject.h"

using namespace entt;
using std::vector;

namespace carlos {
    class SceneSystem : public IGameLoopSystem {
        private:
            registry entityRegistry;
            // vector<GameObject*> gameObjects;
        public:
            SceneSystem();
            ~SceneSystem();

            int getOrder() { return -1000; }

            registry& getEntityRegistry() { return entityRegistry; }
    };
}
