#include "systems/CSharpTestSystem.h"

namespace carlos {
    string_t CSharpTestSystem::getCurrentDirectory(int argCount, char_t** args) {
        //Get the current executable's directory
        //This assumes the managed assembly to load and its runtime configuration file are next to the host
        char_t rootPath[MAX_PATH];
        #if defined(WINDOWS)
            int size = GetFullPathNameW(args[0], sizeof(rootPath) / sizeof(char_t), rootPath, nullptr);
            if (size <= 0)
                return STR("");
        #else
            char* result = realpath(args[0], rootPath);
            if (result == nullptr)
                return STR("");
        #endif

        string_t rootPathStr = rootPath;
        for (size_t i = 0; i < rootPathStr.length(); i++) {
            if (rootPathStr[i] == '\\')
                rootPathStr[i] = '/';
        }

        int index = rootPathStr.find_last_of('/');
        rootPathStr = rootPathStr.substr(0, index);
        
        PLATFORM_OUT << "current directory = " << rootPathStr << endl;
        return rootPathStr;
    }

    CSharpTestSystem::CSharpTestSystem() {
        input = DefaultServiceLocator::getSystem<IInputSystem>();

        AppSystem* app = DefaultServiceLocator::getSystem<AppSystem>();
        folderPath = getCurrentDirectory(app->getArgCount(), app->getArgs());
    }

    void CSharpTestSystem::update() {
        if (input->getKeyDown(Key::SPACE)) {
            runManagedCode(folderPath);
        }
    }
}
