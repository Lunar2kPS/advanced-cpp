#include <stdio.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

//Helpers:
void printLuaStackContents(lua_State* lua);

//SEE: Lua C API YouTube Tutorial Series: https://www.youtube.com/watch?v=xrLQ0OXfjaI&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
void example3LuaStack();

void customExampleLuaFile();

//SEEALSO: https://edw.is/using-lua-with-cpp/
//SEEALSO: https://www.codingwiththomas.com/blog/a-lua-c-api-cheat-sheet
int main() {
    printf("--- %s ---\n", LUA_VERSION);
    example3LuaStack();
    customExampleLuaFile();
    return 0;
}

void printLuaStackContents(lua_State* lua) {
    int stackSize = lua_gettop(lua);
    printf("Stack size: %d\n", stackSize);
    for (int i = -1; i >= -stackSize; i--) {
        int type = lua_type(lua, i);
        printf("    [%d] ", i);

        switch (type) {
            case LUA_TNIL:
                printf("nil\n");
                break;
            case LUA_TNUMBER:
                if (lua_isinteger(lua, i))
                    printf("%lld\n", lua_tointeger(lua, i));
                else
                    printf("%f\n", lua_tonumber(lua, i));
                break;
            case LUA_TSTRING:
                printf("%s\n", lua_tostring(lua, i));
                break;
            case LUA_TTABLE:
                printf("table\n");
                break;
            case LUA_TFUNCTION:
                printf("function\n");
                break;
            case LUA_TUSERDATA:
                printf("userdata\n");
                break;
            case LUA_TTHREAD:
                printf("thread\n");
                break;
            case LUA_TLIGHTUSERDATA:
                printf("lightuserdata\n");
                break;
            default:
                printf("Unknown.\n");
                break;
        }
    }
}

void example3LuaStack() {
    printf("\n--- Lua Example 3: The Lua Stack ---\n");
    lua_State* lua = luaL_newstate();
    lua_pushinteger(lua, 13);
    lua_pushinteger(lua, 15);
    lua_pushinteger(lua, 17);
    lua_pushinteger(lua, 19);
    lua_pushnil(lua);
    lua_pushinteger(lua, 23);
    lua_pushstring(lua, "This is higher than 13 thru 23!");
    printLuaStackContents(lua);
    lua_close(lua);
}

void customExampleLuaFile() {
    printf("\n--- (Custom) Lua File Example! ---\n");

    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);

    //NOTE: This is an example of SETTING variables from C++:
    lua_pushinteger(lua, 64); //This pushes a value (64) onto the stack.
    lua_setglobal(lua, "otherValue"); //This pops the value from the stack and assigns it to a variable in the Lua state.

    if (luaL_dofile(lua, "example.lua") == LUA_OK) {
        printLuaStackContents(lua);

        //NOTE: This is an example of GETTING variables previously-set from Lua:
        lua_getglobal(lua, "hp");
        int hp = (int) lua_tointeger(lua, -1); //This takes a value from the top of the stack (-1) as an integer, WITHOUT popping it off the stack.
        lua_pop(lua, 1); //This takes ONE value off of the top of the stack.
        printf("(C++) hp = %d", hp);
    } else {
        const char* error = lua_tostring(lua, -1);
        fprintf(stderr, "An error occurred while trying to run example.lua! %s", error);
        lua_pop(lua, 1);
    }
    lua_close(lua);
}
