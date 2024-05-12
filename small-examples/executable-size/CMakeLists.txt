cmake_minimum_required(VERSION 3.19)
project(ExecutableSize VERSION 1.0.0 LANGUAGES C CXX)

# WARNING: This seems to increase my executable size??
# set(CMAKE_INTERPROCEDURAL_OPTIMIZATION True)

# NOTE: Below with target_compile_options(...) and -O3 and /O2 is equivalent to setting this (though I'd set the variable before adding the CMake executable target, above, if doing it the variable way)
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable("${PROJECT_NAME}" main.cpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU)|(Clang)")
    message("Setting optimization level to -O3 for gcc / clang")
    target_compile_options("${PROJECT_NAME}" PRIVATE "-O3")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message("Setting optimization level to /O2 for MSVC")
    target_compile_options("${PROJECT_NAME}" "/O2")
endif()

if (WIN32)
    target_link_options("${PROJECT_NAME}" PRIVATE "/ENTRY:main.cpp")
else()
    # TODO: I wonder how to print out the value of the generator expression below?
    #   $<TARGET_FILE:${PROJECT_NAME}>
    add_custom_command(TARGET "${PROJECT_NAME}"
        POST_BUILD COMMAND strip -v --strip-all $<TARGET_FILE:${PROJECT_NAME}>
        COMMENT "Code stripping..."
    )
endif()
