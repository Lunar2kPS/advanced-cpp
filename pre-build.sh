#!/bin/bash

csFolder="csharp"
csproj="MainCSProj"
config="Release"
framework="net7.0"

echo "Building the C# project..."
dotnet build -f "$framework" -c "$config" "$csFolder"
exitCode=$?

if [ $exitCode -ne 0 ]; then
    exit $exitCode
fi

printf "#!/bin/bash
csproj=\"$csproj\"
buildFolder=\"$csFolder/$csproj/bin/$config/$framework\"
" > temp-cs-build-instructions.sh
