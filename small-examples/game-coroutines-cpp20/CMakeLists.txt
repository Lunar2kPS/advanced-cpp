cmake_minimum_required(VERSION 3.19)
project("GameCoroutinesC++20" VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
target_compile_options("${PROJECT_NAME}" PUBLIC /await:strict)
