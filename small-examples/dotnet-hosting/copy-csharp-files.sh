#!/bin/bash

# COMMAND LINE USAGE:
# copy-csharp-files.sh {CONFIG}

argCount=$#
args=("$@")
config=${args[0]}
csprojName="MainCSProj"
cp "../../csharp/$csprojName/bin/$config/net7.0/$csprojName"* ./out/$config
