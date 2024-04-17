#pragma once

namespace carlos {
    /// @brief Represents a player with a name.
    /// @note This is an example C++ class, contained in the library to be exported and available to the calling main program that dynamically links against this library.
    class Player {
        private:
            const char* name;
        public:
            // TODO: Support for MacOS/Linux
            __declspec(dllexport) Player(const char* name);
            __declspec(dllexport) const char* getName() const;
    };
}
