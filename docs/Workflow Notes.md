# Workflow Notes
Note that the steps described in this document assume the following setup:
- VS Code
- Git Bash

## Easy Navigation to .NET Installation
I [wrote a script](/open-dotnet-folder.sh) (still a work-in-progress) that helps you find your .NET installation, if it exists. This is useful because we need some of the C/C++ header files and library files for .NET hosting, on each platform, to run C# code over C++.

_(Our copy of the header files and libraries can be found under /libraries/nethosting in this Git repo)._

Basically, you can find it by using `which dotnet` to see if it's installed on your machine/command-line environment, which tells you the file path of `dotnet.exe`, if it's recognized. The [helper script](/open-dotnet-folder.sh) opens an OS file explorer to the location, to make it easy to copy/paste the files into this project, when needed.
