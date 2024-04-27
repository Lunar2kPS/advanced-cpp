#pragma once

#include "platforms.h"

#if defined(__cplusplus)
    #define EXPORT_LINKAGE_TYPE     extern "C"
#else
    #define EXPORT_LINKAGE_TYPE
#endif

#if defined(WINDOWS)
    #define CALLING_CONVENTION      __cdecl
    #define EXPORT_ATTRIBUTE        __declspec(dllexport)
#else
    #define CALLING_CONVENTION      __attribute__((cdecl))
    #define EXPORT_ATTRIBUTE        __attribute((visibility("default")))
#endif

#define HEADER_EXPORT                           EXPORT_ATTRIBUTE
#define IMPLEMENTATION_EXPORT(returnType)       EXPORT_LINKAGE_TYPE EXPORT_ATTRIBUTE returnType CALLING_CONVENTION
#define IMPLEMENTATION_EXPORT_CTOR              EXPORT_LINKAGE_TYPE EXPORT_ATTRIBUTE CALLING_CONVENTION
