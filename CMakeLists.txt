cmake_minimum_required(VERSION 3.19)
project("AdvancedC++" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_VERBOSE_MAKEFILE ON)

include("Platforms.cmake")
setup_platforms()
# include_platform_directories(SRC_FILES)

# if (UNIX)
#     set(CMAKE_CXX_COMPILER g++)
# endif()

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

target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/json")

# Allows us to use #include statements starting from this folder:
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting")

# Allows us to use target_link_libraries(...) commands starting from this folder:
target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting")

# Links against the dynamic library nethost.dll.
# NOTE: There IS NO hostfxr.lib.
if (WINDOWS)
target_link_libraries("${PROJECT_NAME}" PUBLIC nethost)
elseif (UNIX)
    # This linker option is required when using the dynamic library functions (dlfcn.h):
    target_link_options("${PROJECT_NAME}" PUBLIC -ldl)

    # This linker option is required for the runtime to fully work,
    #       Otherwise it complains failed to load libhostpolicy.so,
    #       and undefined symbol "pthread_create".
    target_link_options("${PROJECT_NAME}" PUBLIC -lpthread)
    
    target_link_libraries("${PROJECT_NAME}" PUBLIC "libnethost.a")
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

foreach (dllFilePath "${DLL_FILES}")
    # Older syntax:
    # get_filename_component(dllName "${dllFilePath}" NAME)
    cmake_path(GET dllFilePath FILENAME dllName)

    # WARNING: For some reason, this copy command doesn't work unless we provide the absolute path:
    #       Maybe because we're calling the cmake executable, which exists under our program files/OS installation files.
    set(absoluteDllFilePath "${CMAKE_CURRENT_SOURCE_DIR}/${dllFilePath}")
    add_custom_command(TARGET "CopyDLLs"
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${absoluteDllFilePath}" "${CMAKE_CURRENT_BINARY_DIR}/${dllName}"
        DEPENDS "${absoluteDllFilePath}"
    )
endforeach()

add_dependencies("${PROJECT_NAME}" "CopyDLLs")
