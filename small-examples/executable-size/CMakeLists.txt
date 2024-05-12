cmake_minimum_required(VERSION 3.19)
project(ExecutableSize VERSION 1.0.0 LANGUAGES C CXX)

add_executable("${PROJECT_NAME}" main.cpp)

target_link_options("${PROJECT_NAME}" PRIVATE "/ENTRY:main")
