cmake_minimum_required(VERSION 3.19)
project("AdvancedC++" VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# Links against the (static or import?) library nethost.lib.
# NOTE: There IS NO hostfxr.lib.
target_link_libraries("${PROJECT_NAME}" PUBLIC nethost.lib)
