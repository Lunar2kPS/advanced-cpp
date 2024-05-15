#!/bin/bash

source "./config.sh"
cmake . -B out -D CMAKE_BUILD_TYPE=$config && \
    cmake --build out --config $config \
