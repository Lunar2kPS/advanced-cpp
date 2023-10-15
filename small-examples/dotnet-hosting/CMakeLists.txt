cmake_minimum_required(VERSION 3.19)
project("DotnetHosting" VERSION 1.0.0)

add_executable("${PROJECT_NAME}" main.cpp)

include("../SmallExamples.cmake")

target_include_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64)|((amd)|(AMD)64)")
    set(INTEL64 True)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "(arm)|(aarch)64")
    set(ARM64 True)
endif()

add_custom_target("CopyDLLs")
if (WIN32)
    if (INTEL64)
        # Allows our CMakeLists.txt file to use target_link_libraries(...) commands starting from this folder:
        target_link_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting/windows/intel64")

        set("dllFilePath" "${LIBRARIES_FOLDER}/nethosting/windows/intel64/nethost.dll")
        if (IS_ABSOLUTE "${dllFilePath}")
            set(absoluteDllFilePath "${dllFilePath}")
        else()
            set(absoluteDllFilePath "${CMAKE_CURRENT_SOURCE_DIR}/${dllFilePath}")
        endif()

        
    endif()

    # Links against the dynamic library nethost.dll.
    # NOTE: There IS NO hostfxr.lib.
    target_link_libraries("${PROJECT_NAME}" PUBLIC nethost)
elseif (APPLE)
    if (INTEL64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting/macos/intel64")
    elseif (ARM64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting/macos/arm64")
    endif()
    target_link_libraries("${PROJECT_NAME}" PUBLIC "libnethost.dylib")
elseif (UNIX)
    if (INTEL64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting/linux/intel64")
    elseif (ARM64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "${LIBRARIES_FOLDER}/nethosting/linux/arm64")
    endif()

    # This linker option is required when using the dynamic library functions (dlfcn.h):
    # target_link_options("${PROJECT_NAME}" PUBLIC -ldl)
    target_link_libraries("${PROJECT_NAME}" PUBLIC dl)

    # This linker option is required for the runtime to fully work,
    #       Otherwise it complains failed to load libhostpolicy.so,
    #       and undefined symbol "pthread_create".
    # target_link_options("${PROJECT_NAME}" PUBLIC -lpthread)
    target_link_libraries("${PROJECT_NAME}" PUBLIC pthread)
    
    target_link_libraries("${PROJECT_NAME}" PUBLIC "libnethost.so")
endif()
add_dependencies("${PROJECT_NAME}" "CopyDLLs")

if (DEFINED dllFilePath)
    # WARNING: For some reason, this copy command doesn't work unless we provide the absolute path:
    #   Maybe because we're calling the cmake executable, which exists under our program files/OS installation files.
    cmake_path(GET dllFilePath FILENAME dllName)
    add_custom_command(TARGET "CopyDLLs"
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${absoluteDllFilePath}" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${dllName}"
        DEPENDS "${absoluteDllFilePath}"
    )
endif()
