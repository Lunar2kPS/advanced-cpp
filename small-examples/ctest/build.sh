#!/bin/bash
config="Debug"
cmake . -B out \
    -D CMAKE_BUILD_TYPE=$config \
    -D EXAMPLE_BUILD_TESTS=True \

cmake --build out --config $config