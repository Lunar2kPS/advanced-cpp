#!/bin/bash

source "./cmake-scripts/get-platform.sh"

case "$simpleOSName" in
    "Windows")
        # NOTE: We could've used something like:
        #   which dotnet | xargs dirname
        # But if the folder path contains spaces, it xargs will send 2+ args over to the dirname command.

        # To fix this, use:
        #   which dotnet | xargs -I {} dirname "{}"
        # The -I specifies that the replacement string, {}, will be replaced by the input (the result of the "which dotnet" command).
        # The quotes around {} allow us to take that entire command output from "which dotnet", and put quotes around them like we normally would, to make it 1 arg.

        # However, on Windows/Git Bash environments, the "/c/Program Files" (POSIX) style of file paths don't work with explorer.exe.
        #   Luckily, there's a "cygwin -w" command to convert to Windows-style file paths that explorer.exe works with,
        #   But the previously-simple, 1-line command, was getting out of hand.
        #   Therefore, I wrote this bash script instead.
        folderPath="$(which dotnet)"
        folderPath="$(dirname "$folderPath")"
        folderPath="$(cygpath -w "$folderPath")"

        explorer "$folderPath"
        ;;
    "MacOS")
        # which dotnet | xargs -I {} dirname "{}" | xargs open

        folderPath="$(which dotnet)"
        if [[ $folderPath == "" ]]; then
            printf ".NET SDK (\"dotnet\") not found in command-line.\nIt's either not installed, or just not in your \$PATH variable.\n"
            exit 1
        fi

        folderPath="$(dirname "$folderPath")"
        open "$folderPath"
        printf "Found .NET SDK in the folder:\n    $folderPath\n"
        ;;
    "Linux")
        ;;
    "Unknown")

        ;;
esac
