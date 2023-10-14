cmake_minimum_required(VERSION 3.19)
project("OpenGLExample" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message("C = ${CMAKE_C_COMPILER}")
message("CXX = ${CMAKE_CXX_COMPILER}")

include("../SmallExamples.cmake")

add_executable("${PROJECT_NAME}" main.cpp)


# NOTE: For the add_subdirectory(...) commands below,
#   We had to specify a 2nd arg (binary_dir) because the LIBRARIES_FOLDER is NOT a subfolder under the current directory.
#   See also: https://cmake.org/cmake/help/latest/command/add_subdirectory.html

# Usually, it's just something like this, with just 1 arg:
#   add_subdirectory("${LIBRARIES_FOLDER}/glad")

# GLAD
add_subdirectory("${LIBRARIES_FOLDER}/glad" "${CMAKE_CURRENT_BINARY_DIR}/libraries/glad")
target_link_libraries("${PROJECT_NAME}" PUBLIC glad)
target_include_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/glad/include")

# GLFW
# This turns OFF building the GLFW tests, examples and documentation:
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${LIBRARIES_FOLDER}/glfw" "${CMAKE_CURRENT_BINARY_DIR}/libraries/glfw")
target_link_libraries("${PROJECT_NAME}" PUBLIC glfw)
target_include_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/glfw/include")
