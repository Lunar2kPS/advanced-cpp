#pragma comment(lib,"hostfxr.lib")
#pragma comment(lib,"nethost.lib")

//SEE: https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting
#include "platforms.h"

#include <iostream>
#include <string>

#if defined(WINDOWS)
#include <Windows.h>
#endif

//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/nethost/nethost.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/coreclr_delegates.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/hostfxr.h
#include <nethost.h>
#include <coreclr_delegates.h>
#include <hostfxr.h>

using std::cout;
using std::cerr;
using std::hex;
using std::showbase;
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

    int runManagedCode(const string_t& rootPath) {
        //STEP 1: Load hostfxr and get certain exported functions
        if (!loadHostfxr()) {
            return 1;
        }

        //STEP 2: Initialize and start the .NET runtime
        const string_t configPath = rootPath + L"MainCSProj.runtimeconfig.json";
        load_assembly_and_get_function_pointer_fn netLoadAssembly = getNETLoadAssembly(configPath.c_str());

        if (netLoadAssembly == nullptr) {
            cerr << "Failed to get .NET load assembly." << endl;
            return 1;
        }

        //STEP 3: Load managed assembly and get pointer to a managed method
        const string_t libraryPath = rootPath + L"MainCSProj.dll";
        const char_t* type = L"MainCSProj.Program, Program";
        const char_t* method = L"SimpleMethod";

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
            L"Message from C++",
            27
        };
        entryPoint(&args, sizeof(args));

        return 0;
    }

    // @brief Attempts to discover the location of hostfxr and get exports, using the nethost library.
    bool loadHostfxr() {
        char_t buffer[MAX_PATH];
        size_t bufferSize = sizeof(buffer) / sizeof(char_t);
        // #if defined(__cplusplus)
        //     01239
        // #endif

        cout << "bufferSize = " << bufferSize << " vs. " << MAX_PATH << endl;
        int result = get_hostfxr_path(buffer, &bufferSize, nullptr);
        cout << "bufferSize = " << bufferSize << " vs. " << MAX_PATH << endl;

        void* library = loadLibrary(buffer);
        initFunction = (hostfxr_initialize_for_runtime_config_fn) getExport(library, "hostfxr_initialize_for_runtime_config");
        getRuntimeDelegateFunction = (hostfxr_get_runtime_delegate_fn) getExport(library, "hostfxr_get_runtime_delegate");
        closeFunction = (hostfxr_close_fn) getExport(library, "hostfxr_close");

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
