cmake_minimum_required(VERSION 3.19)
project(simdjson VERSION 3.6.4 LANGUAGES CXX)

add_library("${PROJECT_NAME}"
    "simdjson.cpp"
)
# target_include_directories("${PROJECT_NAME}" PUBLIC "../include")
