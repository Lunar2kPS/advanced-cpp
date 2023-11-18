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

    /// @brief Represents a purely empty object in the game engine.
    /// GameObjects are allowed to have any number of Components added onto them,
    /// to allow dynamic behavior to be added to them, and potentially mixed and matched.
    class GameObject {
        private:
            string name;
            vector<Component*> components;

        public:
            /// @brief Creates a new GameObject with a given name.
            /// @param name The name of the new object. Note that the GameObject takes ownership over the string provided (given that it's an R-value reference (string&&)).
            GameObject(string&& name) : name(move(name)) { }
            ~GameObject();

            string& getName() { return name; }

            vector<Component*>& getAllComponents() { return components; }

            /// @brief Gets an existing component on the GameObject.
            /// @tparam T The type of component to search for.
            /// @return A pointer to the component that was found, or nullptr otherwise.
            template <typename T>
            T* getComponent();

            /// @brief Adds a new component (using the type's default, empty constructor) to this GameObject.
            /// @note Components add functionality to GameObjects, which are otherwise behaviorless, empty containers.
            /// @tparam T The type of new component to add.
            /// @return A pointer to the newly-added component.
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
