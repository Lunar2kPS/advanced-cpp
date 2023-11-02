#pragma once

#include <string>
#include <vector>
#include <type_traits>

#include "systems/SceneSystem.h"

using std::string;
using std::vector;
using std::enable_if;
using std::is_base_of;
using std::move;

namespace carlos {
    //NOTE: this breaks the circular dependency of GameObject ←-→ Component
    class Component;

    class GameObject {
        private:
            string name;
            vector<Component*> components;

        public:
            GameObject(string&& name) : name(move(name)) { }
            ~GameObject();

            string& getName() { return name; }

            vector<Component*>& getAllComponents() { return components; }

            template <typename T>
            T* getComponent();

            template <typename T>
            T* addComponent();
    };

    template <typename T>
    T* GameObject::getComponent() {
        for (int i = 0; i < components.size(); i++) {
            T* specificType = dynamic_cast<T*>(components[i]);
            if (specificType != nullptr)
                return specificType;
        }
        return nullptr;
    }

    template <typename T>
    T* GameObject::addComponent() {
        T* newComponent = new T();

        //TODO: Generic constraints..
        Component* c = dynamic_cast<Component*>(newComponent);
        if (c != nullptr)
            c->setGameObject(this);

        components.push_back(newComponent);
        return newComponent;
    }
}
