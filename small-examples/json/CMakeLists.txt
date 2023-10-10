cmake_minimum_required(VERSION 3.19)
project("JsonExample" VERSION 1.0.0)

include("../SmallExamples.cmake")

add_executable("${PROJECT_NAME}" main.cpp)
target_include_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}")
