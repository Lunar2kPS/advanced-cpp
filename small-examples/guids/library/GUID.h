#pragma once

#include <iostream>

using std::ostream;

#define EMPTY_GUID "00000000-0000-0000-0000-000000000000"

namespace carlos {
    extern "C" {
        class GUID {
            private:
                char characters[37] = EMPTY_GUID;
            public:
                /// @brief Creates an empty GUID.
                GUID() { }

                GUID(const char (&source)[37]) {
                    strncpy(characters, source, 36);
                    characters[36] = 0;
                }

                /// @brief Creates a new unique GUID.
                __declspec(dllexport) static GUID createNew();

                bool isEmpty() { return strcmp(characters, EMPTY_GUID) == 0; }

                __declspec(dllexport) friend ostream& operator <<(ostream& output, const GUID& guid);
        };
    }
}
