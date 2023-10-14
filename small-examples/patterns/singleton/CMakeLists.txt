cmake_minimum_required(VERSION 3.19)
project("SingletonExample" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable("${PROJECT_NAME}" main.cpp)
