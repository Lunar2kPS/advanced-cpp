cmake_minimum_required(VERSION 3.19)
project("Win32SocketsExample" VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")

# WSA (Windows Sockets API):
target_link_libraries("${PROJECT_NAME}" PUBLIC Ws2_32.lib)
