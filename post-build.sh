#!/bin/bash

# COMMAND LINE USAGE:
# post-build.sh {BUILD_EXIT_CODE} {BUILD_OUTPUT_FOLDER}
argCount=$#
args=("$@")
buildExitCode=${args[0]}
buildOutputFolder=${args[1]}
if [ $buildExitCode -eq 0 ]; then
    success=true
else
    success=false
fi

instructionsFile="$(pwd)/temp-cs-build-instructions.sh"

if [ -f "$instructionsFile" ]; then
    source "$instructionsFile"
    rm "$instructionsFile"
    filesToCopy=(
        "$csproj.exe"
        "$csproj.dll"
        "$csproj.pdb"
        "$csproj.deps.json"
        "$csproj.runtimeconfig.json"
    )

    for fileName in "${filesToCopy[@]}"; do
        cp "$buildFolder/$fileName" $buildOutputFolder
    done
fi
