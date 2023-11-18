#pragma once

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

//SEE: https://learn.microsoft.com/en-us/dotnet/core/tutorials/netcore-hosting

//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/nethost/nethost.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/coreclr_delegates.h
//Provided from: https://github.com/dotnet/runtime/blob/main/src/native/corehost/hostfxr.h
// #define NETHOST_USE_AS_STATIC //<-- for use with libnethost.a instead of libnethost.so
#include "nethost.h"
#include "coreclr_delegates.h"
#include "hostfxr.h"

using std::string;

using string_t = std::basic_string<char_t>;

namespace carlos {
    static hostfxr_initialize_for_runtime_config_fn initFunction;
    static hostfxr_get_runtime_delegate_fn getRuntimeDelegateFunction;
    static hostfxr_close_fn closeFunction;
    static hostfxr_initialize_for_dotnet_command_line_fn init_for_cmd_line_fptr;
    static hostfxr_run_app_fn run_app_fptr;

    static bool didLoad = false;
    static load_assembly_and_get_function_pointer_fn netLoadAssembly = nullptr;

    //NOTE: Huge thanks to the sample at:
    //      https://github.com/dotnet/samples/blob/main/core/hosting/src/NativeHost/nativehost.cpp
    bool loadHostfxr();
    void* loadLibrary(const char_t* path);
    void* getExport(void* library, const char* name);
    load_assembly_and_get_function_pointer_fn getNETLoadAssembly(const char_t* configPath);
    int runManagedCode(const string_t& rootPath);
}
