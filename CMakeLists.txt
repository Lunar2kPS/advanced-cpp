cmake_minimum_required(VERSION 3.19)
project("AdvancedC++" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_VERBOSE_MAKEFILE ON)

include("Platforms.cmake")
setup_platforms()
# include_platform_directories(SRC_FILES)

file(GLOB_RECURSE RESOURCES_FILES "resources/*")

message("Copying resources...")
foreach(filePath ${RESOURCES_FILES})
    string(REGEX REPLACE "${PROJECT_SOURCE_DIR}\/*" "" filePath "${filePath}")
    message("Copying ${filePath}...")

    configure_file(
        "${PROJECT_SOURCE_DIR}/${filePath}"
        "${PROJECT_BINARY_DIR}/${filePath}"
        COPYONLY
    )
endforeach()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")

# Allows our C++ (.cpp) files to use #include statements starting from this folder:
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries")
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting")

# Allows our CMakeLists.txt file to use target_link_libraries(...) commands starting from this folder:
target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting")

# Links against the dynamic library nethost.dll.
# NOTE: There IS NO hostfxr.lib.
if (WINDOWS)
    target_link_libraries("${PROJECT_NAME}" PUBLIC nethost)
elseif (UNIX)
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

# NOTE: These are automatically prepended with the source dir (CMAKE_CURRENT_SOURCE_DIR)
if (WINDOWS)
    set(DLL_FILES
        "libraries/nethosting/nethost.dll"
        # Add more DLL file paths as needed here!
    )
elseif (UNIX)
    set(DLL_FILES
        "libraries/nethosting/libnethost.so"
        # Add more DLL file paths as needed here!
    )
endif()

add_custom_target("CopyDLLs")

# WARNING: NO QUOTES IS IMPORTANT HERE!
#   The list is semi-colon-separated,
#   And the semi-colons need to be OUTSIDE of quotes to separate each iteration of the loop.
foreach (dllFilePath ${DLL_FILES})
    # Older syntax:
    # get_filename_component(dllName "${dllFilePath}" NAME)
    cmake_path(GET dllFilePath FILENAME dllName)
    if (IS_ABSOLUTE "${dllFilePath}")
        set(absoluteDllFilePath "${dllFilePath}")
    else()
        set(absoluteDllFilePath "${CMAKE_CURRENT_SOURCE_DIR}/${dllFilePath}")
    endif()

    # WARNING: For some reason, this copy command doesn't work unless we provide the absolute path:
    #       Maybe because we're calling the cmake executable, which exists under our program files/OS installation files.
    add_custom_command(TARGET "CopyDLLs"
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${absoluteDllFilePath}" "${CMAKE_CURRENT_BINARY_DIR}/${dllName}"
        DEPENDS "${absoluteDllFilePath}"
    )
endforeach()
add_dependencies("${PROJECT_NAME}" "CopyDLLs")

# WARNING: For some reason, this just doesn't work.
#   It complains about non-existent configurations for Release|x64 and Debug|x64...
#   Calling the C# build .sh script from command line (before/after CMake is invoked) works.
#   No idea why.
# add_custom_target("BuildCSharp")
# add_custom_command(
#     TARGET "BuildCSharp"
#     PRE_BUILD
#     COMMAND sh build-csharp.sh
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#     # DEPENDS build-csharp.sh "csharp/MainCSProj/Program.cs" # TODO: Specify all C# scripts under the csharp folder?
# )
# add_dependencies("${PROJECT_NAME}" "BuildCSharp")
