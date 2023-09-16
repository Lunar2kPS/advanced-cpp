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

# find_package(Mono REQUIRED)
# include_directories("${MONO_INCLUDE_DIRS}")
# target_link_libraries("${PROJECT_NAME}" "${MONO_LIBRARIES}")

# message("1")
# file(GLOB_RECURSE MONO_SRC_FILES "libraries/mono/include/*.h" "libraries/mono/include/*.def")
# message("2")
# add_library(mono SHARED "${MONO_SRC_FILES}")
# set_target_properties(mono PROPERTIES LINKER_LANGUAGE CXX)
# message("3")
# target_link_libraries("${PROJECT_NAME}" mono)
# message("4")

target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/json")
# message("5")


# add_subdirectory("libraries/json")
# target_link_libraries("${PROJECT_NAME}" PUBLIC json)
# target_include_directories("${PROJECT_NAME}" PUBLIC "libraries/mono")

# message("6")
# target_include_directories("${PROJECT_NAME}" SYSTEM PUBLIC libraries/mono/include)
# message("7")
# target_link_directories("${PROJECT_NAME}" PUBLIC libraries/mono/lib)
# target_link_libraries("${PROJECT_NAME}" PUBLIC libmono-static-sgen.lib)
