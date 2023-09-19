#include <iostream>
#include <string>

#include "tests/json.h"
#include "basicnethosting.h"

using std::wcout;
using std::cout;
using std::endl;

#if defined(WINDOWS)
int __cdecl wmain(int argCount, wchar_t** args) {
#else
int main(int argCount, char** args) {
#endif
    cout << "Hello world!" << endl;
    testMethod();

    //Get the current executable's directory
    //This assumes the managed assembly to load and its runtime configuration file are next to the host
    char_t rootPath[MAX_PATH];
    #if defined(WINDOWS)
        int size = GetFullPathNameW(args[0], sizeof(rootPath) / sizeof(char_t), rootPath, nullptr);
        if (size <= 0)
            return 1;
        rootPath = windowsPath;
    #else
        char* result = realpath(args[0], rootPath);
        if (result == nullptr)
            return 1;
    #endif
    cout << "rootPath = " << rootPath << endl;

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
