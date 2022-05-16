PROJECT(feedbacksynth_debug)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

# This is only really meant to work on macOS right now
if (APPLE)
    set(RTAUDIO_API_CORE ON CACHE INTERNAL "use OSX_CORE!")
endif (APPLE)

set(RTAUDIO_BUILD_TESTING OFF CACHE INTERNAL "dont build tests!")
set(RTAUDIO_BUILD_STATIC_LIBS OFF CACHE INTERNAL "dont compile library static lib.")
set(RTAUDIO_TARGETNAME_UNINSTALL "uninstall_rtaudio" CACHE INTERNAL "uninstall rtaudio name" FORCE)

include_directories(../lib/DaisySP)
add_subdirectory(../lib/DaisySP DaisySP)

include_directories(../lib/rtaudio)
add_subdirectory(../lib/rtaudio rtaudio)

add_executable(feedbacksynth feedbacksynth_macos.cpp)
add_dependencies(feedbacksynth rtaudio DaisySP)

target_link_libraries(feedbacksynth rtaudio DaisySP)
