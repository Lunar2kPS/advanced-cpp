#include "Player.h"

namespace carlos {
    IMPLEMENTATION_EXPORT_CTOR Player::Player(const char* name) : name(name) { }
    IMPLEMENTATION_EXPORT(const char*) Player::getName() const { return name; }
}
