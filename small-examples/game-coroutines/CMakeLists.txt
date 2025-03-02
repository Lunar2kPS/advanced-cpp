cmake_minimum_required(VERSION 3.19)
project("GameCoroutines" VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
