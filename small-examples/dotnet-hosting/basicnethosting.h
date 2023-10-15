//SEE: https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting
#include "platforms.h"

#include <iostream>
#include <string>

#if defined(WINDOWS)
    #include <Windows.h>
    #include <libloaderapi.h>
    #define STR(s) L ## s
#else
    #include <dlfcn.h>
    #include <limits.h> //<-- This defines PATH_MAX for UNIX systems!
    #define STR(s) s
    #define MAX_PATH PATH_MAX
#endif

//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/nethost/nethost.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/coreclr_delegates.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/hostfxr.h
// #define NETHOST_USE_AS_STATIC //<-- for use with libnethost.a instead of libnethost.so
#include "nethost.h"
#include "coreclr_delegates.h"
#include "hostfxr.h"

using std::cout;
using std::wcout;
using std::cerr;
using std::hex;
using std::dec;
using std::showbase;
using std::noshowbase;
using std::endl;
using std::string;

using string_t = std::basic_string<char_t>;

namespace carlos {
    hostfxr_initialize_for_runtime_config_fn initFunction;
    hostfxr_get_runtime_delegate_fn getRuntimeDelegateFunction;
    hostfxr_close_fn closeFunction;

    hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr;
    hostfxr_run_app_fn run_app_fptr;

    //NOTE: Huge thanks to the sample at:
    //      https://github.com/dotnet/samples/blob/main/core/hosting/src/NativeHost/nativehost.cpp
    void* loadLibrary(const char_t* path);
    void* getExport(void* library, const char* name);
    load_assembly_and_get_function_pointer_fn getNETLoadAssembly(const char_t* configPath);
    bool loadHostfxr();

    string_t getCurrentDirectory(int argCount, char_t** args) {
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
        
        //TODO: Clean this up...
        #if defined(WINDOWS)
            wcout << "rootPath = " << rootPathStr << endl;
        #else
            cout << "rootPath = " << rootPathStr << endl;
        #endif
        return rootPathStr;
    }

    bool didLoad = false;
    load_assembly_and_get_function_pointer_fn netLoadAssembly = nullptr;
    int runManagedCode(const string_t& rootPath) {
        //STEP 1: Load hostfxr and get certain exported functions
        if (!didLoad) {
            if (!loadHostfxr()) {
                return 1;
            }

            //STEP 2: Initialize and start the .NET runtime
            const string_t configPath = rootPath + STR("/MainCSProj.runtimeconfig.json");
            netLoadAssembly = getNETLoadAssembly(configPath.c_str());

            if (netLoadAssembly == nullptr) {
                cerr << "Failed to get .NET load assembly." << endl;
                return 1;
            }
            didLoad = true;
        }

        //STEP 3: Load managed assembly and get pointer to a managed method
        const string_t libraryPath = rootPath + STR("/MainCSProj.dll");
        const char_t* type = STR("MainCSProjNamespace.Program, MainCSProj");
        const char_t* method = STR("SimpleMethod");

        component_entry_point_fn entryPoint = nullptr;
        int result = netLoadAssembly(
            libraryPath.c_str(),
            type,
            method,
            nullptr,
            nullptr,
            (void**) &entryPoint
        );

        if (result != 0 || entryPoint == nullptr) {
            cerr << "Failed to load assembly and main entry point." << endl;
            return 1;
        }

        //STEP 4: Run the managed code!
        struct LibraryArgs {
            const char_t* message;
            int number;
        };

        LibraryArgs args = LibraryArgs {
            STR("Message from C++"),
            27
        };
        entryPoint(&args, sizeof(args));

        return 0;
    }

    // @brief Attempts to discover the location of hostfxr and get exports, using the nethost library.
    bool loadHostfxr() {
        char_t buffer[MAX_PATH];
        size_t bufferSize = sizeof(buffer) / sizeof(char_t);

        //WARNING: get_hostfxr_path returns error code 0x80008083 and nothing in the buffer on Zorin Ubuntu Linux with the .NET 7+ installed via snap install!
        //WEIRD!!
        //THIS ANSWER WAS EXACTLY IT!
        //      https://stackoverflow.com/a/72023264/22587574
        //      I was getting a return value of 0x80008083 (-2147450749), which, according to
        //          this page (https://github.com/dotnet/runtime/blob/main/docs/design/features/host-error-codes.md),
        //          is consistent with the StackOverflow answer: CoreHostLibMissingFailure
        //  FOR SOME REASON, "sudo snap install dotnet-sdk" was incomplete.
        //  The dotnet installation actually works by the Linux scripting install, instructions at:
        //      /docs/Dotnet Installation.md
        //      (https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#scripted-install)
        int result = get_hostfxr_path(buffer, &bufferSize, nullptr);
        if (result != 0)
            cerr << "(ERROR) get_hostfxr_path result = " << hex << showbase << result << dec << noshowbase << endl;
        //TODO: Clean this up...
        #if defined(WINDOWS)
            wcout << "hostfxr path = " << ((wchar_t*) buffer) << endl;
        #else
            cout << "hostfxr path = " << ((char_t*) buffer) << endl;
        #endif
        void* library = loadLibrary(buffer);

        initFunction = (hostfxr_initialize_for_runtime_config_fn) getExport(library, "hostfxr_initialize_for_runtime_config");
        getRuntimeDelegateFunction = (hostfxr_get_runtime_delegate_fn) getExport(library, "hostfxr_get_runtime_delegate");
        closeFunction = (hostfxr_close_fn) getExport(library, "hostfxr_close");

        if (initFunction == nullptr)
            cerr << "initFunction was not found." << endl;
        if (getRuntimeDelegateFunction == nullptr)
            cerr << "getRuntimeDelegateFunction was not found." << endl;
        if (closeFunction == nullptr)
            cerr << "closeFunction was not found." << endl;

        return initFunction != nullptr
            && getRuntimeDelegateFunction != nullptr
            && closeFunction != nullptr
        ;
    }
    
    #if defined(WINDOWS)
        void* loadLibrary(const char_t* path) {
            HMODULE library = LoadLibraryW(path);
            //assert(library != nullptr);
            return (void*) library;
        }

        void* getExport(void* library, const char* name) {
            void* func = GetProcAddress((HMODULE) library, name);
            //assert(func != nullptr);
            return func;
        }
    #else
        void* loadLibrary(const char_t* path) {
            void* library = dlopen(path, RTLD_LAZY | RTLD_LOCAL);
            //assert(library != nullptr);
            return library;
        }

        void* getExport(void* library, const char* name) {
            void* func = dlsym(library, name);
            //assert(func != nullptr);
            return func;
        }
    #endif

    load_assembly_and_get_function_pointer_fn getNETLoadAssembly(const char_t* configPath) {
        void* function = nullptr;
        hostfxr_handle c = nullptr;
        int result = initFunction(configPath, nullptr, &c);
        if (result != 0 || c == nullptr) {
            cerr << "Initialization failed: " << hex << showbase << result << endl;
            closeFunction(c);
            return nullptr;
        }

        result = getRuntimeDelegateFunction(c, hdt_load_assembly_and_get_function_pointer, &function);
        if (result != 0 || function == nullptr)
            cerr << "Getting the delegate failed: " << hex << showbase << result << endl;
        
        closeFunction(c);
        return (load_assembly_and_get_function_pointer_fn) function;
    }
}
