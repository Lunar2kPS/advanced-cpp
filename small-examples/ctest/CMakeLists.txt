cmake_minimum_required(VERSION 3.19)
project(CTestExample VERSION 3.19 LANGUAGES C CXX)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")
target_include_directories("${PROJECT_NAME}" PUBLIC src)

if (BUILD_TESTS)
    enable_testing()

    # WARNING: Hmm.. Ew, I'd either need to make a SHARED/STATIC library
    #   with the common code that I want to test, or... directly add the .cpp files in:
    list(APPEND COMMON_TEST_FILES "${SRC_FILES}")
    list(FILTER COMMON_TEST_FILES EXCLUDE REGEX "src/main.cpp")

    set(SKILL_TESTS_FILES "${COMMON_TEST_FILES}")
    list(APPEND SKILL_TESTS_FILES "tests/skilltests.cpp")
    message("SKILL_TESTS_FILES = ${SKILL_TESTS_FILES}")

    add_executable(SkillTests "${SKILL_TESTS_FILES}")
    add_test(
        NAME SkillTests
        COMMAND SkillTests
    )
endif()
