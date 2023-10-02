function(setup_platforms)
    if (WIN32)
        set(WINDOWS True PARENT_SCOPE)
        set(PLATFORM_NAME "Windows" PARENT_SCOPE)
    elseif (APPLE)
        set(MACOS True PARENT_SCOPE)
        set(PLATFORM_NAME "MacOS" PARENT_SCOPE)
    elseif (UNIX)   # AND NOT APPLE
        set(LINUX True PARENT_SCOPE)
        set(PLATFORM_NAME "Linux" PARENT_SCOPE)

        execute_process(COMMAND "./cmake-scripts/pi-check.sh" RESULT_VARIABLE EXIT_CODE)
        if (NOT EXIT_CODE EQUAL 0)
            set(RASPBERRYPI True PARENT_SCOPE)
        endif()
    else()
    # NOTE: This check doesn't seem to work since we're setting the PLATFORM_NAME variable in PARENT_SCOPE.
    # if (NOT DEFINED PLATFORM_NAME)
        set(PLATFORM_NAME "Unknown" PARENT_SCOPE)
    endif()

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x84_64)|((amd)|(AMD)64)")
        set(INTEL64 True PARENT_SCOPE)
    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        set(ARM64 True PARENT_SCOPE)
    endif()
endfunction()

function(include_platform_directories OUT_VAR)
    message("CMake is building for Platform = ${PLATFORM_NAME}")
    file(GLOB_RECURSE RETURN_VALUE "src/*.cpp")
    list(FILTER RETURN_VALUE EXCLUDE REGEX "src/Platforms/.*")

    if (PLATFORM_NAME STREQUAL "Unknown")
        message("[Warning] The current platform is unknown.")
    else()
        set(PLATFORM_DIR "src/Platforms/${PLATFORM_NAME}")
        message("Including platform-specific folder:    ${PLATFORM_DIR}")

        message("Recursing ${PLATFORM_DIR}/*.cpp...")
        file(GLOB_RECURSE PLATFORM_FILES "${PLATFORM_DIR}/*.cpp")
        list(APPEND RETURN_VALUE "${PLATFORM_FILES}")
    endif()

    set("${OUT_VAR}" "${RETURN_VALUE}" PARENT_SCOPE)
endfunction()
