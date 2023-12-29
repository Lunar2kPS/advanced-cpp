cmake_minimum_required(VERSION 3.19)
project("MainProgram" VERSION 1.0.0)
set(DYNAMIC_LIBRARY_NAME "DynamicLibraryExample")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("../SmallExamples.cmake")

add_executable("${PROJECT_NAME}" main.cpp)

add_library("${DYNAMIC_LIBRARY_NAME}" SHARED "library/SomeClass.h")
set_target_properties("${DYNAMIC_LIBRARY_NAME}" PROPERTIES LINKER_LANGUAGE CXX)
