cmake_minimum_required(VERSION 3.19)
project("AdvancedC++" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_VERBOSE_MAKEFILE ON)

include("cmake/Platforms.cmake")
setup_platforms()
# include_platform_directories(SRC_FILES)

include("cmake/Resources.cmake")
copy_resources_assets()

file(GLOB_RECURSE SRC_FILES "src/*.cpp")


# Dear ImGui
# See: https://github.com/ocornut/imgui/wiki/Getting-Started
file(GLOB IMGUI_FILES "libraries/imgui/*.cpp")
list(APPEND IMGUI_FILES
    "libraries/imgui/backends/imgui_impl_glfw.cpp"
    "libraries/imgui/backends/imgui_impl_opengl3.cpp"
    # "libraries/imgui/misc/cpp/imgui_stdlib.cpp"
)
list(APPEND SRC_FILES ${IMGUI_FILES})
add_executable("${PROJECT_NAME}" "${SRC_FILES}")


# Allows our C++ (.cpp) files to use #include statements starting from this folder:
target_include_directories("${PROJECT_NAME}" PUBLIC "src")
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries")
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting")

# Allows our CMakeLists.txt file to use target_link_libraries(...) commands starting from this folder:
if (WINDOWS)
    if (INTEL64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting/windows/intel64")
    endif()
elseif (MACOS)
    if (INTEL64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting/macos/intel64")
    endif()
    # TODO: Support M1/M2 (Apple Silicon) ARM architecture!
elseif (LINUX)
    if (INTEL64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting/linux/intel64")
    elseif (ARM64)
        target_link_directories("${PROJECT_NAME}" PUBLIC "libraries/nethosting/linux/arm64")
    endif()
endif()


# GLAD
add_subdirectory("libraries/glad")
target_link_libraries("${PROJECT_NAME}" PUBLIC glad)
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/glad/include")


# GLFW
# This turns OFF building the GLFW tests, examples and documentation:
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# SEE: Note from https://www.glfw.org/docs/latest/build_guide.html#build_link_cmake_source
# This adds a "glfw" target, from their CMakeLists.txt file.
add_subdirectory("libraries/glfw")

# This links GLFW to our executable!
target_link_libraries("${PROJECT_NAME}" PUBLIC glfw)

# We need this for our C++ program to find the .h header files when building:
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/glfw/include")


# GLM
# See for glm compiler options: https://chromium.googlesource.com/external/github.com/g-truc/glm/+/HEAD/manual.md
add_compile_definitions(GLM_FORCE_LEFT_HANDED)
add_subdirectory("libraries/glm")
target_link_libraries("${PROJECT_NAME}" PUBLIC glm)
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/glm")


# Dear ImGui
# add_subdirectory("cmake/imgui")
# target_link_libraries("${PROJECT_NAME}" PUBLIC imgui)
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/imgui")
target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/imgui/backends")


if (WINDOWS)
    # Links against the dynamic library nethost.dll.
    # NOTE: There IS NO hostfxr.lib.
    target_link_libraries("${PROJECT_NAME}" PUBLIC nethost)
elseif (MACOS)
    target_link_libraries("${PROJECT_NAME}" PUBLIC "libnethost.dylib")
elseif (LINUX)
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
        "libraries/nethosting/windows/intel64/nethost.dll"
        # Add more DLL file paths as needed here!
    )
elseif (MACOS)
    # TODO: Support M1/M2 (Apple Silicon) ARM architecture!
    set(DLL_FILES
        "libraries/nethosting/macos/intel64/libnethost.dylib"
    )
elseif (LINUX)
    if (INTEL64)
        set(DLL_FILES
            "libraries/nethosting/linux/intel64/libnethost.so"
        )
    elseif (ARM64)
        set(DLL_FILES
            "libraries/nethosting/linux/arm64/libnethost.so"
        )
    endif()
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

    message("Copying ${absoluteDllFilePath}")

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
