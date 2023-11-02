#include "GameObject.h"

namespace carlos {
    GameObject::~GameObject() {
        for (int i = components.size() - 1; i >= 0; i--)
            delete components[i];
    }
}
