cmake_minimum_required(VERSION 3.19)
project(gladGL VERSION 2.0.4 LANGUAGES C)

add_library("${PROJECT_NAME}"
    "../src/gl.c"
)
target_include_directories("${PROJECT_NAME}" PUBLIC "../include")
