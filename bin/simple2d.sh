#!/bin/bash

set -euo pipefail

#-------------------------------------------------------------------------------
# The Simple 2D Command-Line Utility for Unix-like Systems
#-------------------------------------------------------------------------------

# The installed version
VERSION='2.0.0'

#-------------------------------------------------------------------------------
# Variables
#-------------------------------------------------------------------------------

arch=$(uname -m)
platform='unknown'

# Colors
INFO='\033[1;36m'     # cyan, underline
ERROR='\033[1;31m'    # red, underline
NORMAL='\033[0m'      # reset

#-------------------------------------------------------------------------------
# Detect Platform
#-------------------------------------------------------------------------------

unamestr=$(uname)

# macOS
if [[ $unamestr == 'Darwin' ]]; then
  platform='macos'
# Linux (including ARM and Raspberry Pi)
elif [[ $unamestr == 'Linux' ]]; then
  platform='linux'
# Windows / MinGW
elif [[ $unamestr =~ 'MINGW' ]]; then
  platform='mingw'
# BSD
elif [[ $unamestr =~ 'BSD' ]]; then
  platform='bsd'
fi

# Unsupported platform
if [[ $platform == 'unknown' ]]; then
  echo -e "${ERROR}Error:${NORMAL} Not a supported Unix-like system"
  exit 1
fi

#-------------------------------------------------------------------------------
# Main Functions
#-------------------------------------------------------------------------------

# Returns the linker flags and libraries needed for compilation
get_libs() {
  local INCLUDE_DIRS="-I./simple2d/include -I./simple2d/deps/include"
  local arch_dir="./simple2d/deps/$platform/$arch"
  if [[ ! -d "$arch_dir" ]]; then
    arch_dir="./simple2d/deps/$platform/universal"
  fi
  local LIB_DIRS="-L./simple2d/build -L$arch_dir"
  local LIB_FLAGS="-lsimple2d -lSDL3 -lSDL3_image -lSDL3_mixer -lSDL3_ttf"

  if [[ $platform == 'macos' ]]; then
    local MACOS_FRAMEWORKS=(
      AppKit
      AVFoundation
      AudioToolbox
      Carbon
      Cocoa
      CoreAudio
      CoreHaptics
      CoreMedia
      ForceFeedback
      GameController
      IOKit
      Metal
      OpenGL
      QuartzCore
      UniformTypeIdentifiers
    )
    for framework in "${MACOS_FRAMEWORKS[@]}"; do
      LIB_FLAGS="$LIB_FLAGS -Wl,-framework,$framework"
    done
  elif [[ $platform == 'linux' ]]; then
    LIB_FLAGS="$LIB_FLAGS -lm"
  elif [[ $platform == 'mingw' ]]; then
    LIB_FLAGS="$LIB_FLAGS -lhid -lsetupapi -lole32 -loleaut32 -luuid -lgdi32 -luser32 -lwinmm -lusp10 -limm32 -lversion -lrpcrt4"
  fi

  echo "$INCLUDE_DIRS $LIB_DIRS $LIB_FLAGS"
}

# Builds a Simple 2D program, provided a single C or C++ file
build() {
  # Check if provided source file exists
  if [[ ! -f "$1" ]]; then
    echo -e "${ERROR}Error:${NORMAL} File '$1' not found"
    exit 1
  fi

  # # Set the directory this script is run in to the current working directory
  # cd "$(dirname "$0")"
  # echo `pwd`

  # Check if required directories exist
  if [[ ! -d "./simple2d/include" || ! -d "./simple2d/deps/include" || ! -d "./simple2d/build" || ( ! -d "./simple2d/deps/$platform/$arch" && ! -d "./simple2d/deps/$platform/universal" ) ]]; then
    echo -e "${ERROR}Error:${NORMAL} Could not find the Simple 2D includes and libraries. Make sure to recursively clone Simple 2D and build dependencies first. See the project README for instructions."
    exit 1
  fi

  local compiler_cmd
  # Note: get_libs output intentionally unquoted to allow word splitting
  if [[ ${1: -2} == '.c' ]]; then
    compiler_cmd=(${CC:-cc} -std=c11 "$1" $(get_libs) -o "${1%.*}")
  elif [[ ${1: -4} == '.cpp' ]]; then
    compiler_cmd=(${CXX:-c++} -std=c++11 "$1" $(get_libs) -o "${1%.*}")
  else
    echo -e "${ERROR}Error:${NORMAL} File must end in .c or .cpp"
    exit 1
  fi

  if ! "${compiler_cmd[@]}"; then
    echo -e "${ERROR}Error:${NORMAL} Compilation failed"
    exit 1
  fi

  echo -e "${INFO}Info:${NORMAL} Built '${1%.*}' from '${1}'"
}

#-----------------------------------------------------------
# Process command-line arguments
#-----------------------------------------------------------

print_usage() {
  cat << EOF
Usage: simple2d [option]

Options:
  -v, --version  Print the version number
  --libs         Output the libraries needed for compilation
  build [file]   Build a Simple 2D program from a C or C++ source file

Examples:
  simple2d --version
  simple2d --libs
  simple2d build triangle.c

EOF
}

case "${1-}" in
  build)
    [[ -z "${2-}" ]] && { print_usage; exit 1; }
    build "${2-}";;
  --libs)
    get_libs;;
  -v|--version)
    echo "$VERSION";;
  *)
    print_usage;;
esac
