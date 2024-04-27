#pragma once

#include "exports.h"

namespace carlos {
    /// @brief Represents a player with a name.
    /// @note This is an example C++ class, contained in the library to be exported and available to the calling main program that dynamically links against this library.
    class Player {
        private:
            const char* name;
        public:
            // TODO: Support for MacOS/Linux
            HEADER_EXPORT Player(const char* name);
            HEADER_EXPORT const char* getName() const;
    };
}
