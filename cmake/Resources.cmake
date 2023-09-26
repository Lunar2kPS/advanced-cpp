function(copy_resources_assets)
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
endfunction()
