cmake_minimum_required(VERSION 3.19)
project(CTestExample VERSION 3.19 LANGUAGES C CXX)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
target_include_directories("${PROJECT_NAME}" PUBLIC src)
