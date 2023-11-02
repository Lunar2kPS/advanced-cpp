#pragma once

#include "glmutility.h"
#include "components/Component.h"

namespace carlos {
    class Transform : public Component {
        private:
            vec2 localPosition;
        
        public:
            void reset();
            vec2 getLocalPosition() { return localPosition; }
            void setLocalPosition(vec2 value);
    };
}
