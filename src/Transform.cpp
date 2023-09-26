#include "Transform.h"

namespace carlos {
    void Transform::reset() {
        this->localPosition = vec2(0);
    }

    void Transform::setLocalPosition(vec2 localPosition) {
        this->localPosition = localPosition;
    }
}