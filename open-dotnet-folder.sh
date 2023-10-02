#!/bin/bash

# Roughly equivalent to:
#   which dotnet | xargs -I {} dirname "{}"
#   But it was getting complicated, so I wrote a bash script instead:
folderPath="$(which dotnet)"
folderPath="$(dirname "$folderPath")"
folderPath="$(cygpath -w "$folderPath")"

explorer "$folderPath"
