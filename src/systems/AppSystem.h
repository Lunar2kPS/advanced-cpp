#pragma once

#include <iostream>

#include "platforms.h"

#include "interfaces/IGameLoopSystem.h"

using std::cout;
using std::wcout;
using std::endl;
using std::cerr;

namespace carlos {
    #if defined(WINDOWS)
        typedef wchar_t platform_char;
        typedef wchar_t platform_char_t;
        #define PLATFORM_OUT wcout
    #else
        typedef char platform_char;
        typedef char_t platform_char_t;
        #define PLATFORM_OUT cout
    #endif

    class AppSystem : public IGameLoopSystem {
        private:
            int argCount;
            platform_char** args;
            bool quitFlag;

        public:
            int getArgCount() { return argCount; }
            platform_char** getArgs() { return args; }

            AppSystem(int argCount, platform_char** args) : argCount(argCount), args(args) { }
            bool isQuitRequested() { return quitFlag; }
            bool requestQuit() {
                quitFlag = true;
                return true;
            }
    };
}
