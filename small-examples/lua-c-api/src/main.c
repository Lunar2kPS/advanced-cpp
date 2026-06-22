#include <stdio.h>

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"

//Helpers:
void printLuaStackContents(lua_State* lua);

//SEE: Lua C API YouTube Tutorial Series: https://www.youtube.com/watch?v=xrLQ0OXfjaI&list=PLLwK93hM93Z3nhfJyRRWGRXHaXgNX0Itk
void example3LuaStack();
void example4LuaFunctions();
void example5LuaFunctionParams();

void customExampleLuaFile();

//SEEALSO: Lua 5.5 Manual: https://lua.org/manual/5.5/
//  This includes the Lua 5.5 C API reference.
//SEEALSO: https://edw.is/using-lua-with-cpp/
//SEEALSO: https://www.codingwiththomas.com/blog/a-lua-c-api-cheat-sheet
int main() {
    printf("--- %s ---\n", LUA_VERSION);
    example3LuaStack();
    example4LuaFunctions();
    example5LuaFunctionParams();
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

    luaL_dostring(lua, "x = 42");
    int result = lua_getglobal(lua, "x");   //This pushes the value of "x" (variable) onto the top of the stack!
    lua_Integer x = lua_tointeger(lua, -1); //This gets a value off the stack, as an integer.
    lua_remove(lua, -1);                    //This removes a value off the stack, effectively cleaning up what we just did.
    printf("result = %lld\n", x);

    lua_close(lua);
}

void example4LuaFunctions() {
    printf("\n--- Lua Example 4: Functions ---\n");
    
    lua_State* lua = luaL_newstate();
    const char* filePath = "lua/Example 4 - Functions.lua";
    if (luaL_dofile(lua, filePath) == LUA_OK) {
        printLuaStackContents(lua);
        lua_getglobal(lua, "return4"); //+1 on stack (function)
        printLuaStackContents(lua);
        if (lua_isfunction(lua, -1)) {
            //SEE:  https://lua.org/manual/5.5/manual.html#lua_call
            //      https://lua.org/manual/5.5/manual.html#lua_pcall

            //This calls a function on the Lua C Stack, with its args immediately after it on the stack:
            // CONTENTS     INDEX
            //  argN        -1
            //  ...         ...
            //  arg2
            //  arg1
            //  arg0
            //  function
            //FORM: lua_pcall(lua_State* L, int nargs, int nresults, int msgh);
            lua_pcall(lua, 0, 1, 0);    //-1 on stack (function)
                                        //+1 on stack (int -- return value)
            printLuaStackContents(lua);
            int result = lua_tointeger(lua, -1);
            printf("return4() => %d\n", result);
            lua_remove(lua, -1); //-1 on stack (int -- return value)
        }
    } else {
        const char* error = lua_tostring(lua, -1);
        fprintf(stderr, "An error occurred while trying to run %s! %s\n", filePath, error);
    }
    lua_close(lua);
}

void example5LuaFunctionParams() {
    printf("\n--- Lua Example 5: Function Parameters ---\n");

    lua_State* lua = luaL_newstate();
    const char* filePath = "lua/Example 5 - Function Params.lua";
    if (luaL_dofile(lua, filePath) == LUA_OK) {
        lua_getglobal(lua, "pythagoras"); //+1 on stack (function)
        const int ARG_COUNT = 2;
        const int RETURN_COUNT = 1;
        lua_pushnumber(lua, 3); //+1 on stack (int)
        lua_pushnumber(lua, 4); //+1 on stack (int)
        printLuaStackContents(lua);
        lua_pcall(lua, ARG_COUNT, RETURN_COUNT, 0); //-3 on stack (function, int, int)
                                                    //+1 on stack (int -- return value)
        printLuaStackContents(lua);
        int result = lua_tointeger(lua, -1);
        lua_remove(lua, -1);
        printf("pythagoras(3, 4) => %d\n", result);
    } else {
        const char* error = lua_tostring(lua, -1);
        fprintf(stderr, "An error occurred while trying to run %s! %s\n", filePath, error);
    }
    lua_close(lua);
}

void customExampleLuaFile() {
    printf("\n--- (Custom) Lua File Example! ---\n");

    lua_State* lua = luaL_newstate();
    luaL_openlibs(lua);

    //NOTE: This is an example of SETTING variables from C++:
    lua_pushinteger(lua, 64); //This pushes a value (64) onto the stack.
    lua_setglobal(lua, "otherValue"); //This pops the value from the stack and assigns it to a variable in the Lua state.

    const char* filePath = "lua/Custom Example.lua";
    if (luaL_dofile(lua, filePath) == LUA_OK) {
        printLuaStackContents(lua);

        //NOTE: This is an example of GETTING variables previously-set from Lua:
        lua_getglobal(lua, "hp");
        int hp = (int) lua_tointeger(lua, -1); //This takes a value from the top of the stack (-1) as an integer, WITHOUT popping it off the stack.
        lua_pop(lua, 1); //This takes ONE value off of the top of the stack.
        printf("(C++) hp = %d", hp);
    } else {
        const char* error = lua_tostring(lua, -1);
        fprintf(stderr, "An error occurred while trying to run %s! %s\n", filePath, error);
        lua_pop(lua, 1);
    }
    lua_close(lua);
}
