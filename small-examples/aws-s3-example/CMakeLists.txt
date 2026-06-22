# NOTE: To build this, use:
# cmake -S . -B out -D CMAKE_BUILD_TYPE=Debug -D AWSSDK_DIR="C:/Program Files (x86)/aws-cpp-sdk-all/lib/cmake/AWSSDK" && cmake --build out --config=Debug

cmake_minimum_required(VERSION 3.28)
project("C++AWSS3Example" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
target_include_directories("${PROJECT_NAME}" PUBLIC "C:/Program Files (x86)/aws-cpp-sdk-all/include")

# NOTE: The below find_package(...) command requires that you build AWS S3 yourself,
#   and run the following to build and install it on your machine:
# NOTE: You MUST run the terminal as administrator if you don't change the CMAKE_INSTALL_PREFIX (because it will default to C:/Program Files or C:/Program Files (x86), for example).
# git clone --recursive https://github.com/aws/aws-sdk-cpp
# cd aws-sdk-cpp
# cmake . -B out -D CMAKE_BUILD_TYPE=Debug -D BUILD_ONLY="s3" -D ENABLE_TESTING=OFF -D AUTORUN_UNIT_TESTS=OFF
# cmake --build out --config=Debug
# cmake --install out --config=Debug

set(CMAKE_PREFIX_PATH "C:/Program Files (x86)/aws-cpp-sdk-all")
find_package(AWSSDK REQUIRED COMPONENTS s3)

# NOTE: Use this for debugging:
# get_property(allTargets DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY BUILDSYSTEM_TARGETS)
# get_property(importedTargets GLOBAL PROPERTY IMPORTED_TARGETS)
# message(STATUS "Targets: ${allTargets}")
# message(STATUS "Imported: ${importedTargets}")

# NOTE: AWS might not provide a clear way to differentiate between when they do and do NOT use the AWS:: prefix for their library names, so let's check both to be sure we can build our project:
if(TARGET AWS::aws-cpp-sdk-s3)
    target_link_libraries("${PROJECT_NAME}" AWS::aws-cpp-sdk-s3)
else()
    target_link_libraries("${PROJECT_NAME}"
        aws-cpp-sdk-core
        aws-cpp-sdk-s3
    )
endif()

# NOTE: On Windows, the linker needs the .lib files, but at runtime, the OS needs the .dll files. We must copy the DLLs in (if that's how we're going to pull in the AWS S3 library for now).
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "C:/Program Files (x86)/aws-cpp-sdk-all/bin"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()
