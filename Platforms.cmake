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
        
        set(filePath "pi-check.sh")
        string(REGEX REPLACE "src/" "" toFilePath "${filePath}")
        configure_file(
            "${PROJECT_SOURCE_DIR}/${filePath}"
            "${PROJECT_BINARY_DIR}/${toFilePath}"
            COPYONLY
        )
    else()
    # NOTE: This check doesn't seem to work since we're setting the PLATFORM_NAME variable in PARENT_SCOPE.
    # if (NOT DEFINED PLATFORM_NAME)
        set(PLATFORM_NAME "Unknown" PARENT_SCOPE)
    endif()
endfunction()

# function(include_platform_directories OUT_VAR)
#     message("CMake is building for Platform = ${PLATFORM_NAME}")
#     file(GLOB_RECURSE RETURN_VALUE "src/*.cpp")
#     list(FILTER RETURN_VALUE EXCLUDE REGEX "src/Platforms/.*")

#     if (PLATFORM_NAME STREQUAL "Unknown")
#         message("[Warning] The current platform is unknown.")
#     else()
#         set(PLATFORM_DIR "src/Platforms/${PLATFORM_NAME}")
#         message("Including platform-specific folder:    ${PLATFORM_DIR}")

#         message("Recursing ${PLATFORM_DIR}/*.cpp...")
#         file(GLOB_RECURSE PLATFORM_FILES "${PLATFORM_DIR}/*.cpp")
#         list(APPEND RETURN_VALUE "${PLATFORM_FILES}")
#     endif()

#     set("${OUT_VAR}" "${RETURN_VALUE}" PARENT_SCOPE)
# endfunction()