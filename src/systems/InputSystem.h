#pragma once

#include <unordered_map>

#include "interfaces/IInputSystem.h"
#include "interfaces/IWindowSystem.h"

using std::unordered_map;

namespace carlos {
    class InputSystem : public IInputSystem {
        private:
            static const int KEY_COUNT = 118;

            struct KeyState {
                Key key;
                bool wasPressed;

                KeyState(Key key) : key(key), wasPressed(false) { }
            };

            IWindowSystem* windowing;
            int keyCount;
            KeyState* keyStates;
            unordered_map<Key, int> keyLookup;

            void saveKeySnapshot();
        public:
            InputSystem();
            ~InputSystem();
            bool getKeyDown(Key key);
            bool getKey(Key key);
            bool getKeyUp(Key key);

            void postRender();
    };
}
