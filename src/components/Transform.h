#pragma once

#include "glmutility.h"
#include "components/Component.h"

namespace carlos {
    /// @brief A basic component that allows you to set a 2D position on a GameObject.
    /// @note This is used in all sorts of systems, most notably rendering.
    class Transform : public Component {
        private:
            vec2 localPosition;
        
        public:
            /// @brief Resets this Transform back to the world origin.
            void reset();

            /// @brief Gets this Transform's position. Note that no Transform parenting/hierarchies are currently supported yet.
            vec2 getLocalPosition() { return localPosition; }

            /// @brief Gets this Transform's position. Note that no Transform parenting/hierarchies are currently supported yet.
            void setLocalPosition(vec2 value);
    };
}
