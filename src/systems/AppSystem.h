#pragma once

#include <iostream>

#include "platforms.h"
#include "nethost.h"

#include "interfaces/IGameLoopSystem.h"

using std::cout;
using std::wcout;
using std::endl;
using std::cerr;

namespace carlos {
    #if defined(WINDOWS)
        #define PLATFORM_OUT wcout
    #else
        #define PLATFORM_OUT cout
    #endif

    class AppSystem : public IGameLoopSystem {
        private:
            int argCount;
            char_t** args;
            bool quitFlag;

        public:
            int getArgCount() { return argCount; }
            char_t** getArgs() { return args; }

            AppSystem(int argCount, char_t** args) : argCount(argCount), args(args) { }
            bool isQuitRequested() { return quitFlag; }
            bool requestQuit() {
                quitFlag = true;
                return true;
            }
    };
}
