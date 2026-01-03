#include <iostream>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lauxlib.h"
    #include "lua/lualib.h"
}

using std::cout;
using std::cerr;
using std::endl;

int main() {
    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);

    if (luaL_dofile(lua, "example.lua") != LUA_OK) {
        const char* error = lua_tostring(lua, -1);
        cerr << "An error occurred while trying to run example.lua! " << error << endl;
        lua_pop(lua, 1);
    }

    lua_close(lua);

    cout << "Lua C Example Program!" << endl;
    return 0;
}
