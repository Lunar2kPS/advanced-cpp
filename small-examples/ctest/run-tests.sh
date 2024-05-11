#!/bin/bash
cd out
config="Debug"
ctest -C $config -V
cd -
