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

    //NOTE: This is an example of SETTING variables from C++:
    lua_pushinteger(lua, 64); //This pushes the value onto the stack.
    lua_setglobal(lua, "otherValue"); //This pops the value from the stack and assigns it to a variable in the Lua state/script.

    if (luaL_dofile(lua, "example.lua") == LUA_OK) {
        //NOTE: This is an example of GETTING variables previously-set from Lua:
        lua_getglobal(lua, "hp");
        int hp = (int) lua_tointeger(lua, -1); //This takes a value from the top of the stack (-1) as an integer, WITHOUT popping it off the stack.
        lua_pop(lua, 1); //This takes ONE value off of the top of the stack.
        cout << "(C++) hp = " << hp << endl;
    } else {
        const char* error = lua_tostring(lua, -1);
        cerr << "An error occurred while trying to run example.lua! " << error << endl;
        lua_pop(lua, 1);
    }
    lua_close(lua);

    cout << "Lua C Example Program!" << endl;
    return 0;
}
