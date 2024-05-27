cmake_minimum_required(VERSION 3.19)
project("C++Multithreading" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
