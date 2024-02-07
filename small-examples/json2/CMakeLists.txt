cmake_minimum_required(VERSION 3.19)
project("JsonExample2" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("../SmallExamples.cmake")

add_executable("${PROJECT_NAME}" main.cpp)
target_include_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}")
