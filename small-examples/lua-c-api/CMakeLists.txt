cmake_minimum_required(VERSION 3.19)
project("LuaExample" VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23) # NOTE: C standards include C17 and C23. There is no C20 like there is C++20.
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.c")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")

# Resources-like file that gets copied alongside the build executable:
message(STATUS "Copying example.lua...")
configure_file(
    "${PROJECT_SOURCE_DIR}/example.lua"
    "${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/example.lua"
    COPYONLY
)

include("../SmallExamples.cmake")

# Lua C API:
file(GLOB LUA_SRC_FILES "${LIBRARIES_FOLDER}/lua/*.c")
set(LUA_IGNORE_FILES
    "onelua.c"  # This includes the entire build, and is not meant to be included with everything else.
    "lua.c"     # This contains their main program if you were going to use the Lua interpreter.
    "ltests.c"  # This is mostly for internal Lua use, but can help with debugging with LUA_DEBUG symbol.
)

# IMPORTANT: NO STRING QUOTES for the list here, so it expands to individual elements:
foreach (ignoreFile ${LUA_IGNORE_FILES})
    list(FILTER LUA_SRC_FILES EXCLUDE REGEX ".*${ignoreFile}$")
endforeach()

add_library(lua STATIC "${LUA_SRC_FILES}")
target_link_libraries("${PROJECT_NAME}" PRIVATE lua)
target_include_directories("${PROJECT_NAME}" PRIVATE "${LIBRARIES_FOLDER}")
