cmake_minimum_required(VERSION 3.19)
project("X11Windowing" VERSION 1.0.0 LANGUAGES C CXX)

add_executable("${PROJECT_NAME}" main.cpp)

target_link_libraries("${PROJECT_NAME}" PUBLIC X11)
