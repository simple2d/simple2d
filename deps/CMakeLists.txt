cmake_minimum_required(VERSION 3.16)

project(simple2d_deps)

# Disable shared libraries (static builds only)
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)

# SDL_image
set(SDLIMAGE_VENDORED ON)
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP ON)
set(SDLIMAGE_GIF ON)
set(SDLIMAGE_JPG ON)
set(SDLIMAGE_LBM OFF)
set(SDLIMAGE_PCX OFF)
set(SDLIMAGE_PNG ON)
set(SDLIMAGE_PNM OFF)
set(SDLIMAGE_QOI OFF)
set(SDLIMAGE_SVG ON)
set(SDLIMAGE_TGA OFF)
set(SDLIMAGE_TIF OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_XCF OFF)
set(SDLIMAGE_XPM OFF)
set(SDLIMAGE_XV OFF)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)

# SDL_mixer
if(CMAKE_SYSTEM_NAME STREQUAL "Linux"
   AND CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  # Work around because `mpg123` doesn't currently build on Linux/Arm64
  set(SDLMIXER_VENDORED OFF)
else()
  set(SDLMIXER_VENDORED ON)
endif()
set(SDLMIXER_FLAC ON)
set(SDLMIXER_GME OFF)
set(SDLMIXER_MOD OFF)
set(SDLMIXER_MP3 ON)
if(MINGW)
  # Prefer dr_mp3 on Windows/MinGW ARM64 to avoid libmpg123 POSIX cpu-probe code
  set(SDLMIXER_MP3_DRMP3 ON  CACHE BOOL "" FORCE)
  set(SDLMIXER_MP3_MPG123 OFF CACHE BOOL "" FORCE)
endif()
if(EMSCRIPTEN)
  # Use dr_mp3 instead of mpg123 for Emscripten (mpg123 requires ASM)
  set(SDLMIXER_MP3_DRMP3 ON  CACHE BOOL "" FORCE)
  set(SDLMIXER_MP3_MPG123 OFF CACHE BOOL "" FORCE)
endif()
set(SDLMIXER_MIDI OFF)
set(SDLMIXER_MIDI_NATIVE OFF)
set(SDLMIXER_OPUS OFF)
set(SDLMIXER_WAVE ON)
set(SDLMIXER_WAVPACK OFF)
add_subdirectory(SDL_mixer EXCLUDE_FROM_ALL)

# SDL_ttf
set(SDLTTF_VENDORED ON)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

# Build all static libraries as part of the default target
add_custom_target(deps ALL DEPENDS
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_mixer::SDL3_mixer
  SDL3_ttf::SDL3_ttf
)
