cmake_minimum_required(VERSION 3.19)
project("MainProgram" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("Platforms.cmake")
setup_platforms()

set(LIBRARY_NAME "DynamicLibraryExample")
add_library("${LIBRARY_NAME}" SHARED "library/Player.cpp")

add_executable("${PROJECT_NAME}" main.cpp)

# NOTE: On Windows, this only works when you have EXPORTS in your C/C++ code!
target_link_libraries("${PROJECT_NAME}" PUBLIC DynamicLibraryExample)

if (MACOS)
    set_target_properties("${LIBRARY_NAME}" PROPERTIES
        # OUTPUT "DynamicLibraryExample"
        PREFIX ""
        SUFFIX ".dylib"
    )
endif()

if (UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    # NOTE: Or you can also use the following to generate PIC for a certain target, like ${LIBRARY_NAME}:
    # set_target_properties(${LIBRARY_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

# NOTE: The install_name_tool on MacOS uses XCode behind the scenes and takes this form, for our use case of changing dependency search path:
#   install_name_tool -change old_dependency_path new_dependency_path your_dylib.dylib

add_custom_command(
    TARGET "${PROJECT_NAME}"
    POST_BUILD
    COMMAND install_name_tool -change "@rpath/${LIBRARY_NAME}.dylib" "${LIBRARY_NAME}.dylib" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}"
)
