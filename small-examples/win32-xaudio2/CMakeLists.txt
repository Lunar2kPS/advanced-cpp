cmake_minimum_required(VERSION 3.19)
project("Win32XAudio2" VERSION 1.0.0 LANGUAGES C CXX)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable("${PROJECT_NAME}" "${SRC_FILES}")

target_link_libraries("${PROJECT_NAME}" PUBLIC
    user32          # Win32 API
    xaudio2.lib     # XAudio2
)
