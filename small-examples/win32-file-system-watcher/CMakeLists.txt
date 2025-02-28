cmake_minimum_required(VERSION 3.19)
project("Win32FileSystemWatcher" VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
message(STATUS "SRC_FILES = ${SRC_FILES}")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
