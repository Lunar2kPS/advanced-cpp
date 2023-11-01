#pragma once

#include <string>
#include <vector>
#include <type_traits>
#include "entt/entt.hpp"

#include "components/Component.h"
#include "systems/SceneSystem.h"

using namespace entt;
using std::string;
using std::vector;
using std::enable_if;
using std::is_base_of;

namespace carlos {
    class GameObject {
        private:
            string name;
            entity entity;
            GameObject();

        public:
            static GameObject* create(string&& name);
            ~GameObject();

            template <typename T, typename = enable_if<is_base_of<Component, T>::value>>
            T getComponent();
    };

    template <typename T, typename>
    T GameObject::getComponent() {
        
    }
}