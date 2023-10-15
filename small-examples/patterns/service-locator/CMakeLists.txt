cmake_minimum_required(VERSION 3.19)
project("ServiceLocatorExample" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# NOTE: You can use GLOB_RECURSE, but here I don't, because it could grab cpp files from the "out" folder.
#   For a larger project, I'd make a "src" subfolder for all the .cpp/.h files to help avoid this problem.
file(GLOB SRC_FILES "*.cpp")
message("src = ${SRC_FILES}")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
