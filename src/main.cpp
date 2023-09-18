#include <iostream>
#include <string>

#include "tests/json.h"
#include "basicnethosting.h"

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
    int indexA = rootPathStr.find_last_of('/');
    int indexB = rootPathStr.find_last_of('\\');
    int index;
    if (indexA == string::npos) {
        index = indexB;
    } else if (indexB == string::npos) {
        index = indexA;
    } else {
        index = (indexA > indexB) ? indexA : indexB;
    }
    rootPathStr = rootPathStr.substr(0, index + 1);

    carlos::runManagedCode(rootPath);
    return 0;
}
