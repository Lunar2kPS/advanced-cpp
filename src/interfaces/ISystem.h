#pragma once

namespace carlos {
    /// @brief An interface representing a system that can be ordered, compared to other systems.
    class ISystem {
        public:
            virtual int getOrder() { return 0; }
    };
}
