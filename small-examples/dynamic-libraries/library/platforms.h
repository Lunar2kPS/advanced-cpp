#pragma once

#if defined(_WIN32)
    #define WINDOWS
#elif defined(__APPLE__)
    #define MACOS
#elif defined(__linux__)
    //NOTE: RASPBERRY_PI may also be defined from CMake.
    //  See the main CMakeLists.txt, where it uses add_compile_definitions(...)
    #define LINUX
#else
    #define UNKNOWN_PLATFORM
#endif

//NOTE: (MSVC flag || Clang/GCC flag)
#if defined(_M_IX86) || defined(__i386__)
    #define INTEL
    #define BIT32
#elif defined(_M_X64) || defined(__x86_64__)
    #define INTEL
    #define BIT64
#elif defined(_M_ARM) || defined(__arm__)
    #define ARM
    #define BIT32
#elif defined(_M_ARM64) || defined(__aarch64__)
    #define ARM
    #define BIT64
#endif

inline const char* getOSName() {
    return 
#if defined WINDOWS
        "Windows"
#elif defined MACOS
        "MacOS"
#elif defined LINUX
        "Linux"
#else
        "UNKNOWN"
#endif
    ;
}


#if defined(WINDOWS)
    #define PLATFORM_OUT wcout
#else
    #define PLATFORM_OUT cout
#endif


inline const char* getBitness() {
    return
#if defined BIT32
        "32bit"
#elif defined BIT64
        "64bit"
#else
        "UNKNOWN"
#endif
    ;
}

inline const char* getCPUArchitecture() {
    return
#if defined INTEL
        "Intel"
#elif defined ARM
        "Arm"
#else
        "UNKNOWN"
#endif
    ;
}

