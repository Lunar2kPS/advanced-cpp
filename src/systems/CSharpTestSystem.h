#pragma once

#include "basicnethosting.h"

#include "interfaces/IGameLoopSystem.h"
#include "interfaces/IInputSystem.h"
#include "systems/AppSystem.h"
#include "ServiceLocator.h"

namespace carlos {
    class CSharpTestSystem : public IGameLoopSystem {
        private:
            static string_t getCurrentDirectory(int argCount, char_t** args);

        private:
            IInputSystem* input;
            string_t folderPath;
        public:
            CSharpTestSystem();
            
            int getOrder() { return 2000; }
            void update();
    };
}
