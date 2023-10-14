cmake_minimum_required(VERSION 3.19)
project("UTF-8Example" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable("${PROJECT_NAME}" main.cpp)

# NOTE: For UTF-8 Unicode tips,
#   see: http://utf8everywhere.org/
#       Also see their "10. How to do text on Windows" section.
# For VS Code, see: https://learn.microsoft.com/en-us/powershell/scripting/dev-cross-plat/vscode/understanding-file-encoding

# NOTE: This is equivalent to the following C++:
#   #define UNICODE
#   #define _UNICODE
target_compile_definitions("${PROJECT_NAME}" PRIVATE UNICODE _UNICODE)

if (WINDOWS)
    add_compile_options(/utf-8)
elseif (UNIX)
    add_compile_options(-f input-charset=UTF-8)
endif()
