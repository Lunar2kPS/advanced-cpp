cmake_minimum_required(VERSION 3.19)
project("GUIDLibrary" VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include("Platforms.cmake")
setup_platforms()

set(SRC_FILES
    "library/GUID.cpp"
)
add_library("${PROJECT_NAME}" SHARED "${SRC_FILES}")
target_include_directories("${PROJECT_NAME}" PUBLIC library)
if (WINDOWS)
    target_link_libraries("${PROJECT_NAME}" PUBLIC rpcrt4.lib)
endif()

if (EXAMPLE_BUILD_TESTS)
    message("Building CTest unit tests...\n")
    enable_testing()

    set(GUID_TEST_FILES "tests/GUIDtests.cpp")

    add_executable(GUIDTests "${GUID_TEST_FILES}")
    target_link_libraries(GUIDTests PUBLIC "${PROJECT_NAME}")
    target_include_directories(GUIDTests PUBLIC library)

    add_test(
        NAME GUIDTests
        COMMAND GUIDTests
    )
endif()
