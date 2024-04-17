cmake_minimum_required(VERSION 3.19)
project("MainProgram" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(DynamicLibraryExample SHARED "library/Player.cpp")

add_executable("${PROJECT_NAME}" main.cpp)

# NOTE: On Windows, this only works when you have EXPORTS in your C/C++ code!
target_link_libraries("${PROJECT_NAME}" PUBLIC DynamicLibraryExample)
