#include "Player.h"

namespace carlos {
    // TODO: Support for MacOS/Linux
    extern "C" __declspec(dllexport) __cdecl Player::Player(const char* name) : name(name) { }
    extern "C" __declspec(dllexport) const char* __cdecl Player::getName() const { return name; }
}
