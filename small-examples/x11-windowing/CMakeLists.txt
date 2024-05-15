cmake_minimum_required(VERSION 3.19)
project("X11Windowing" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_FILES
    "main.cpp"
)
list(INSERT SRC_FILES 0
    "libraries/glad/src/gl.c"
    "libraries/glad/src/glx.c"
)

add_executable("${PROJECT_NAME}" "${SRC_FILES}")

target_include_directories("${PROJECT_NAME}" PUBLIC
    "src"
    "libraries"
    "libraries/glad/include"
)

target_link_libraries("${PROJECT_NAME}" PUBLIC X11)
target_link_libraries("${PROJECT_NAME}" PUBLIC dl) # NOTE: This is libdl, used for dlopen/dlclose (which is used in C/C++ to load dynamic/shared library code at runtime)
