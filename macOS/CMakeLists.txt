# NOTE: This is only meant to work on macOS right now

PROJECT(feedbacksynth_debug)
cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

### DaisySP ###

include_directories(../lib/DaisySP)
add_subdirectory(../lib/DaisySP DaisySP)

### RtAudio ###

set(RTAUDIO_BUILD_TESTING OFF CACHE INTERNAL "Don't build RtAudio tests!")
set(RTAUDIO_BUILD_STATIC_LIBS ON CACHE INTERNAL "Compile RtAudio static libs")
set(RTAUDIO_BUILD_SHARED_LIBS OFF CACHE INTERNAL "Don't compile RtAudio shared libs")
set(RTAUDIO_TARGETNAME_UNINSTALL "uninstall_rtaudio" CACHE INTERNAL "uninstall rtaudio name" FORCE)
set(RTAUDIO_API_CORE ON CACHE INTERNAL "Use CoreAudio")

include_directories(../lib/rtaudio)
add_subdirectory(../lib/rtaudio rtaudio)

### RtMidi ###

set(RTMIDI_BUILD_TESTING OFF CACHE INTERNAL "Don't build RtMidi tests")
set(RTMIDI_BUILD_STATIC_LIBS ON CACHE INTERNAL "Compile RtMidi static libs")
set(RTMIDI_BUILD_SHARED_LIBS OFF CACHE INTERNAL "Don't compile RtMidi shared libs")
set(RTMIDI_TARGETNAME_UNINSTALL "uninstall_rtmidi" CACHE INTERNAL "uninstall rtmidi name" FORCE)
set(RTMIDI_API_CORE ON CACHE INTERNAL "Use CoreMidi")

include_directories(../lib/rtmidi)
add_subdirectory(../lib/rtmidi rtmidi)

### Main ###

include_directories(../Source)

add_executable(
    feedbacksynth
    FeedbackSynth_macos.cpp
    ../Source/BiquadFilters.cpp
    ../Source/FeedbackSynthEngine.cpp
    ../Source/KarplusString.cpp
)

add_dependencies(feedbacksynth rtaudio rtmidi DaisySP)
target_link_libraries(feedbacksynth rtaudio rtmidi DaisySP)