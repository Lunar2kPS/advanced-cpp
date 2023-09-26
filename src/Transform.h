#pragma once
#include "includers/glm.h"

namespace carlos {
    class Transform {
        private:
            vec2 localPosition;
        
        public:
            void reset();
            vec2 getLocalPosition() { return localPosition; }
            void setLocalPosition(vec2 value);
    };
}
