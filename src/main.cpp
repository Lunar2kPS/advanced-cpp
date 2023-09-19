#include <iostream>
#include <string>

#include "tests/json.h"
#include "basicnethosting.h"

using std::wcout;
using std::cout;
using std::endl;

int __cdecl wmain(int argCount, wchar_t** args) {
    cout << "Hello world!" << endl;
    testMethod();

    //Get the current executable's directory
    //This assumes the managed assembly to load and its runtime configuration file are next to the host
    char_t rootPath[MAX_PATH];
    #if defined(WINDOWS)
        int size = GetFullPathNameW(args[0], sizeof(rootPath) / sizeof(char_t), rootPath, nullptr);
        if (size <= 0)
            return 1;
    #else

    #endif

    string_t rootPathStr = rootPath;
    for (size_t i = 0; i < rootPathStr.length(); i++) {
        if (rootPathStr[i] == '\\')
            rootPathStr[i] = '/';
    }

    int index = rootPathStr.find_last_of('/');
    rootPathStr = rootPathStr.substr(0, index);

    carlos::runManagedCode(rootPathStr);
    return 0;
}
