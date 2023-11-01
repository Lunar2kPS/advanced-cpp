#include "GameObject.h"

#include "ServiceLocator.h"

using std::move;

namespace carlos {
    GameObject* GameObject::create(string&& name) {
        ServiceLocator* locator = ServiceLocator::getInstance();
        if (locator == nullptr)
            return nullptr;
        SceneSystem* scenes = locator->getSystem<SceneSystem>();
        if (scenes == nullptr)
            return nullptr;

        GameObject* gameObject = new GameObject();
        gameObject->name = move(name);

        return gameObject;
    }

    GameObject::~GameObject() {
        
    }


}
