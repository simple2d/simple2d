#!/bin/bash

# ------------------------------------------------------------------------------
# The Simple 2D Command-Line Utility for Unix-like Systems
#
# Run from the web using:
#   bash <(curl -fsSL https://script_url_here)
# or...
#   bash <(wget -qO - https://script_url_here)
# ------------------------------------------------------------------------------

# Set Constants ################################################################

# The installed version
VERSION='0.8.0'

# URL to this script in the repo
SCRIPT_URL="https://raw.githubusercontent.com/simple2d/simple2d/master/bin/simple2d.sh"

# MinGW Simple 2D installer
s2d_mingw_installer_fname="simple2d-windows-mingw-${VERSION}.zip"
s2d_mingw_installer_url="https://github.com/simple2d/simple2d/releases/download/v${VERSION}/${s2d_mingw_installer_fname}"

# SDL download paths
libsdl_url="https://www.libsdl.org"

sdl_fname="SDL2-2.0.7"
sdl_url="${libsdl_url}/release/${sdl_fname}.tar.gz"

image_fname="SDL2_image-2.0.2"
image_url="${libsdl_url}/projects/SDL_image/release/${image_fname}.tar.gz"

mixer_fname="SDL2_mixer-2.0.2"
mixer_url="${libsdl_url}/projects/SDL_mixer/release/${mixer_fname}.tar.gz"

ttf_fname="SDL2_ttf-2.0.14"
ttf_url="${libsdl_url}/projects/SDL_ttf/release/${ttf_fname}.tar.gz"

# SDL config
sdl_arm_config_flags="\
--disable-video-opengl --disable-video-x11 \
--disable-pulseaudio --disable-esd \
--disable-video-mir --disable-video-wayland"

# Colors
BOLD='\033[1;39m'     # default, bold
UNDERLINE='\033[4m'   # underline
TASK='\033[1;34m'     # blue, bold
BLUE=$TASK
INFO='\033[1;36m'     # cyan, underline
WARN='\033[1;33m'     # yellow, underline
ERROR='\033[1;31m'    # red, underline
SUCCESS='\033[1;32m'  # green, bold
NORMAL='\033[0m'      # reset

# Set Variables ################################################################

platform='unknown'
platform_display='unknown'
platform_rpi=false
ret=''  # for storing function return values
confirmed=false # to skip confirmation prompts

# Use xcpretty for nicer output: gem install xcpretty
if xcpretty -v &>/dev/null; then
  XCPRETTY=xcpretty
else
  XCPRETTY=cat
fi

# Helper Functions #############################################################


# Prints an information messages
# params:
#   $1  String  Message text
info_msg() {
  echo -e "${INFO}Info:${NORMAL} $1\n"
}
warning_msg() {
  echo -e "${WARN}Warning:${NORMAL} $1\n"
}
error_msg() {
  echo -e "${ERROR}Error:${NORMAL} $1\n"
}
success_msg() {
  echo -e "${SUCCESS}$1${NORMAL}\n"
}


# Prompts to continue, or exits
# params:
#   $1  String  Message before y/n prompt
prompt_to_continue() {
  if ! $confirmed; then
    printf "${BOLD}$1${NORMAL} "
    read -p "(y/n) " ans
    if [[ $ans != "y" ]]; then
      echo -e "\nQuitting...\n"
      exit
    fi
    echo
  fi
}


# Prints the task in progress
# params:
#   $1  String  Name of task
#   $2  String  Optional white space, e.g. "\n\n"
print_task() {
  printf "${TASK}==>${BOLD} $1${NORMAL}$2"
}


# Prints and runs a command
# params:
#   $1  String  The command
print_and_run() {
  echo "==> $1"; $1
}


# Checks if a library is installed
# params:
#   $1  String  Name of the library, e.g. SDL2, without the "-l"
have_lib?() {
  print_task "Checking for $1... "

  if $(ld -o /dev/null -l$1 2>&1 | grep -qE '(library not found|ld: cannot find -l)'); then
    echo "no"
    return 1
  else
    echo "yes"
    return 0
  fi
}


# Gets the text from a remote resource
# params:
#   $1  String  URL of the resource
get_remote_str() {
  which curl > /dev/null && cmd='curl -fsSL' || cmd='wget -qO -';
  ret=$($cmd $1)
}


# Compares version numbers in the form `#.#.#`
# Adapted from:
#  stackoverflow.com/questions/4023830/bash-how-compare-two-strings-in-version-format
# params:
#   $1  String  First version number to compare
#   $2  String  Second version number to compare
# returns:
#   0  If $1 is equal to $2
#   1  If $1 is newer than $2
#   2  If $1 is older than $2
compare_versions() {
  if [[ $1 == $2 ]]
    then
    return 0
  fi
  local IFS=.
  local i ver1=($1) ver2=($2)
  # fill empty fields in ver1 with zeros
  for ((i=${#ver1[@]}; i<${#ver2[@]}; i++))
  do
    ver1[i]=0
  done
  for ((i=0; i<${#ver1[@]}; i++))
  do
    if [[ -z ${ver2[i]} ]]
      then
      # fill empty fields in ver2 with zeros
      ver2[i]=0
    fi
    if ((10#${ver1[i]} > 10#${ver2[i]}))
      then
      return 1
    fi
    if ((10#${ver1[i]} < 10#${ver2[i]}))
      then
      return 2
    fi
  done
  return 0
}


# Build  #######################################################################


# Builds a Simple 2D program
# If directory contains an Xcode project, it will call `xcodebuild`.
# If $1 is a C or C++ source file, it will attempt to compile using `gcc`.
build() {

  # If no input, print build usage
  if [[ $1 == '' ]]; then print_usage_build; exit; fi

  # If C or C++ source file given, e.g.:
  #   build app.c; build app.cpp
  if [[ ${1: -2} == '.c' || ${1: -4} == '.cpp' ]]; then
    # Compile
    gcc -std=c11 $1 `simple2d --libs` -o ${1%.*}
    exit
  fi

  # Check if current directory has an Xcode project
  has_xcodeproj?() {
    if [[ $(ls *.xcodeproj 2>/dev/null) ]]; then
      return 0
    else
      return 1
    fi
  }

  # Build an Xcode project
  build_with_xcodebuild() {
    if [[ $XCPRETTY != 'xcpretty' ]]; then
      echo -e "xcpretty not found: Run \`gem install xcpretty\` for nicer xcodebuild output.\n"
    fi
    case $1 in
      --ios)
        xcodebuild -sdk iphonesimulator  | $XCPRETTY; exit;;
      --ios-device)
        xcodebuild -sdk iphoneos         | $XCPRETTY; exit;;
      --tvos)
        xcodebuild -sdk appletvsimulator | $XCPRETTY; exit;;
      --tvos-device)
        xcodebuild -sdk appletvos        | $XCPRETTY; exit;;
    esac
  }

  # If bad SDK flag, print build usage
  if ! [[ $1 =~ ^(--ios|--ios-device|--tvos|--tvos-device)$ ]]; then
    print_usage_build; exit 1
  fi

  # If current directory has an Xcode project, e.g.:
  #   build --[ios|tvos][-device]
  if has_xcodeproj?; then
    build_with_xcodebuild $1
  fi

  # If an Xcode project path and options are provided, e.g.:
  #   build --ios[-device] build/ios
  #   build --tvos[-device] build/tvos
  #   build --ios[-device] build/ios/MyApp.xcodeproj
  #   build --tvos[-device] build/tvos/MyApp.xcodeproj
  if [[ ${2: -10} == '.xcodeproj' ]]; then
    cd $(dirname $2)
  elif [[ -d $2 ]]; then
    cd $2
  else
    error_msg "\"$2\" is not a directory or Xcode project"; exit 1
  fi

  if ! has_xcodeproj?; then
    error_msg "The directory does not contain an Xcode project"; exit 1
  fi

  build_with_xcodebuild $1
}


# Installation #################################################################


# Checks if SDL2 libraries are installed
have_sdl2_libs?() {

  have_all_libs=true
  have_sdl2_lib=true
  have_image_lib=true
  have_mixer_lib=true
  have_ttf_lib=true

  if ! have_lib? 'SDL2'; then
    have_sdl2_lib=false
    have_all_libs=false
  fi

  if ! have_lib? 'SDL2_image'; then
    have_image_lib=false
    have_all_libs=false
  fi

  if ! have_lib? 'SDL2_mixer'; then
    have_mixer_lib=false
    have_all_libs=false
  fi

  if ! have_lib? 'SDL2_ttf'; then
    have_ttf_lib=false
    have_all_libs=false
  fi

  if $have_all_libs; then
    return 0
  else
    return 1
  fi
}


# Installs SDL on Linux
install_sdl_linux() {

  echo "The following packages will be installed:"
  echo "  libsdl2-dev"
  echo "  libsdl2-image-dev"
  echo "  libsdl2-mixer-dev"
  echo "  libsdl2-ttf-dev"; echo

  prompt_to_continue "Install SDL now?"

  print_task "Updating packages" "\n\n"
  print_and_run "sudo apt update"
  echo; print_task "Installing packages" "\n\n"
  print_and_run "sudo apt install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
  echo

  if ! have_sdl2_libs?; then
    echo; error_msg "SDL libraries did not install correctly"; exit 1
  else
    echo; info_msg "SDL was installed successfully"
  fi
}


# Installs SDL from source
#   Takes about 30 min on RPI 2
install_sdl_source() {

  echo "SDL will be compiled and installed from source."; echo

  prompt_to_continue "Install SDL now?"

  # Install SDL dependencies
  print_task "Installing SDL2 dependencies" "\n\n"
  sudo apt update

  libs=(
    build-essential  # This may be missing on some ARM platforms

    # SDL2
    libudev-dev
    libdbus-1-dev
    libasound2-dev

    # SDL2_image
    libjpeg9-dev
    libpng-dev

    # SDL2_mixer
    libmpg123-dev
    libvorbis-dev
    libflac-dev

    # SDL2_ttf
    libgl1-mesa-dev  # Solves error missing -lGL
    libfreetype6-dev
  )

  sudo apt install -y ${libs[*]}

  # Downloads, compiles, and installs an SDL library from source
  # params:
  #   $1  String  URL of the tar archive
  #   $2  String  Name of the tar file
  #   $3  String  ./configure flags
  install_sdl_lib () {
    cd /tmp
    wget -N $1
    tar -xzf $2.tar.gz
    cd $2
    print_task "Configuring" "\n\n"
    print_and_run "./configure $3"
    echo; print_task "Compiling" "\n\n"
    make
    echo; print_task "Installing" "\n\n"
    sudo make install
    rm /tmp/$2.tar.gz
    rm -rf /tmp/$2
  }

  # Note that `$have_*_lib` variables are set by `have_sdl2_libs?()`

  if [[ $have_sdl2_lib == 'false' ]]; then
    echo; print_task "Downloading SDL2" "\n\n"
    if [[ $platform == 'arm' ]]; then
      if $platform_rpi; then
        config="--host=arm-raspberry-linux-gnueabihf $sdl_arm_config_flags"
      else
        config=$sdl_arm_config_flags
      fi
      install_sdl_lib $sdl_url $sdl_fname "$config"
    else
      install_sdl_lib $sdl_url $sdl_fname
    fi
  fi

  if [[ $have_image_lib == 'false' ]]; then
    echo; print_task "Downloading SDL2_image" "\n\n"
    install_sdl_lib $image_url $image_fname
  fi

  if [[ $have_mixer_lib == 'false' ]]; then
    echo; print_task "Downloading SDL2_mixer" "\n\n"
    install_sdl_lib $mixer_url $mixer_fname
  fi

  if [[ $have_ttf_lib == 'false' ]]; then
    echo; print_task "Downloading SDL2_ttf" "\n\n"
    install_sdl_lib $ttf_url $ttf_fname
  fi

  echo
  if ! have_sdl2_libs?; then
    error_msg "SDL libraries did not install correctly"; exit 1
  else
    echo; info_msg "SDL was installed successfully"
  fi
}


# Installs SDL
install_sdl() {
  if [[ $platform == 'linux' ]]; then
    install_sdl_linux
  elif [[ $platform == 'arm' ]]; then
    install_sdl_source
  fi
}


# Installs Simple 2D for MinGW environments
install_s2d_mingw() {
  tmp_dir="/tmp/simple2d"
  mkdir -p $tmp_dir
  print_and_run "wget -NP $tmp_dir $s2d_mingw_installer_url"
  print_and_run "pacman -S unzip --needed --noconfirm"
  print_and_run "unzip -q $tmp_dir/$s2d_mingw_installer_fname -d $tmp_dir"
  print_and_run "cd $tmp_dir"
  print_and_run "bash install.sh -y"
  print_and_run "rm -rf $tmp_dir"
}


# Installs Simple 2D
# params:
#   $1  String  Version to install, either 'master' or $VERSION
install_s2d() {

  tmp_dir="/tmp/simple2d"

  mkdir $tmp_dir

  if [[ $1 == 'master' ]]; then
    f_name=$1
  else
    f_name="v$1"
  fi

  url="https://github.com/simple2d/simple2d/archive/$f_name.zip"

  print_task "Downloading Simple 2D" "\n\n"
  # Linux and Raspberry Pi may not have curl installed by default
  if which curl > /dev/null; then
    print_and_run "curl -L $url -o $tmp_dir/$f_name.zip"
  else
    print_and_run "wget -NP $tmp_dir $url"
  fi

  # Check if archive was downloaded properly
  if [[ ! -f "$tmp_dir/$f_name.zip" ]]; then
    echo; error_msg "Simple 2D could not be downloaded"; exit 1
  fi

  echo; print_task "Unpacking" "\n\n"
  print_and_run "unzip -q $tmp_dir/$f_name -d $tmp_dir"

  # Check if archive was unpacked properly
  if [[ $? != 0 ]]; then
    echo; error_msg "Could not unpack. The downloaded Simple 2D package may be damaged."; exit 1
  fi

  print_and_run "cd $tmp_dir/simple2d-$1"

  echo; print_and_run "make"
  echo; print_and_run "sudo make install"

  echo; print_task "Cleaning up" "\n"; echo
  print_and_run "rm -rf $tmp_dir"; echo

  # Check if S2D installed correctly
  if ! have_lib? 'simple2d'; then
    echo; error_msg "Simple 2D did not install correctly"; exit 1
  fi
  echo
}


# Main entry point to install Simple 2D and SDL
# params:
#   $1  String  Flags used, e.g. `--sdl`
install() {

  # If macOS, print message and quit
  if [[ $platform == 'macos' ]]; then
    macos_homebrew_message $1
  fi

  # If SDL flag, install only SDL and quit
  if [[ $1 == '--sdl' ]]; then
    echo
    if have_sdl2_libs?; then
      echo; info_msg "SDL is already installed"
    else
      echo; install_sdl
    fi
    exit
  fi

  # Welcome message
  echo -e "\n${BOLD}Welcome to Simple 2D!${NORMAL}"
  echo -e "${BLUE}---------------------${NORMAL}\n"

  echo -e "Platform detected: ${BOLD}${platform_display}${NORMAL}\n"

  # If MinGW, install Simple 2D release and quit
  if [[ $platform == 'mingw' ]]; then
    prompt_to_continue "Continue to install?"
    install_s2d_mingw
    exit
  fi

  if have_lib? 'simple2d' > /dev/null; then
    warning_msg "Simple 2D is already installed. Proceeding will reinstall."
  fi

  echo -e "Simple 2D will be installed to the following locations:
  /usr/local/include/simple2d.h
  /usr/local/lib/libsimple2d.a
  /usr/local/bin/simple2d"
  echo

  if [[ $1 == '--HEAD' ]]; then
    echo -e "This will install Simple 2D from the \`master\` development branch.\n"
  fi

  prompt_to_continue "Continue?"

  if have_sdl2_libs?; then
    echo
  else
    echo; install_sdl
  fi

  if [[ $1 == '--HEAD' ]]; then
    install_s2d 'master'
  else
    install_s2d $VERSION
  fi

  success_msg "Simple 2D installed successfully!"
}


# Uninstall ####################################################################


# Uninstalls SDL on Linux
uninstall_sdl_linux() {

  echo -e "The following packages will be removed:"
  echo "  libsdl2-dev"
  echo "  libsdl2-image-dev"
  echo "  libsdl2-mixer-dev"
  echo "  libsdl2-ttf-dev"; echo

  prompt_to_continue "Uninstall SDL now?"

  print_task "Uninstalling packages" "\n\n"
  print_and_run "sudo apt remove -y --purge libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
  echo

  if have_sdl2_libs?; then
    echo; error_msg "SDL libraries did not uninstall correctly"
  else
    echo; info_msg "SDL was uninstalled successfully"
  fi
}


# Uninstalls SDL from source
uninstall_sdl_source() {

  prompt_to_continue "Uninstall SDL now?"

  # Uninstall packages in case they were used
  print_and_run "sudo apt remove -y --purge libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"

  uninstall_sdl_lib () {
    cd /tmp
    wget -N $1
    tar -xzf $2.tar.gz
    cd $2
    ./configure $3
    sudo make uninstall
    rm /tmp/$2.tar.gz
    rm -rf /tmp/$2
  }

  echo; print_task "Uninstalling SDL2_image" "\n\n"
  uninstall_sdl_lib $image_url $image_fname

  echo; print_task "Uninstalling SDL2_mixer" "\n\n"
  uninstall_sdl_lib $mixer_url $mixer_fname

  echo; print_task "Uninstalling SDL2_ttf" "\n\n"
  uninstall_sdl_lib $ttf_url $ttf_fname

  echo; print_task "Uninstalling SDL2" "\n\n"
  if [[ $platform == 'arm' ]]; then
    uninstall_sdl_lib $sdl_url $sdl_fname "$sdl_arm_config_flags"
  else
    uninstall_sdl_lib $sdl_url $sdl_fname
  fi

  echo
  if have_sdl2_libs?; then
    echo; error_msg "SDL libraries did not uninstall correctly"; exit 1
  else
    echo; info_msg "SDL was uninstalled successfully"
  fi
}


# Uninstalls SDL
uninstall_sdl() {

  if have_sdl2_libs?; then
    echo
  else
    echo; info_msg "SDL appears to be already uninstalled"
    prompt_to_continue "Try uninstalling SDL anyways?"
  fi

  if [[ $platform == 'linux' ]]; then
    uninstall_sdl_linux
  elif [[ $platform == 'arm' ]]; then
    uninstall_sdl_source
  fi
}


# Uninstalls Simple 2D
# params:
#   $1  String  Flags used, e.g. `--sdl`
uninstall() {

  if [[ $platform == 'macos' ]]; then
    macos_homebrew_message $1
  fi

  # If MinGW, print message and quit
  if [[ $platform == 'mingw' ]]; then
    mingw_not_implemented_message
  fi

  if [[ $1 == '--sdl' ]]; then
    echo; uninstall_sdl
    exit
  fi

  echo; echo -e "The following files will be removed:
  /usr/local/include/simple2d.h
  /usr/local/lib/libsimple2d.a
  /usr/local/bin/simple2d"

  echo; prompt_to_continue "Uninstall Simple 2D?"

  # Use hard-coded, absolute paths for safety
  print_and_run "rm -f /usr/local/include/simple2d.h"
  print_and_run "rm -f /usr/local/lib/libsimple2d.a"
  print_and_run "rm -f /usr/local/bin/simple2d"

  # Check if files were actually deleted
  files=(/usr/local/include/simple2d.h
         /usr/local/lib/libsimple2d.a
         /usr/local/bin/simple2d)

  if [ -f ${files[0]} -o -f ${files[1]} -o -f ${files[2]} ]; then
    echo; error_msg "Simple 2D files could not be removed. Try using \`sudo\`?"
  else
    echo; success_msg "Simple 2D uninstalled!"
  fi
}


# Update #######################################################################


# Updates Simple 2D to latest version or commit
# params:
#   $1  String  Flags used, e.g. `--HEAD`
update() {

  if [[ $platform == 'macos' ]]; then
    macos_homebrew_message
  fi

  # If MinGW, print message and quit
  if [[ $platform == 'mingw' ]]; then
    mingw_not_implemented_message
  fi

  # Check if Simple 2D is installed
  if ! have_lib? 'simple2d' > /dev/null; then
    error_msg "Simple 2D isn't currently installed"
    echo -e "Use the \`install\` command to install Simple 2D.\n"
    exit 1
  fi

  # Check if SDL is installed
  update_check_sdl() {
    if have_sdl2_libs?; then
      echo
    else
      echo; error_msg "SDL libraries missing"
      echo -e "Run \`simple2d install --sdl\` to install SDL.\n"
      exit 1
    fi
  }

  # Read this script from repo, get the version number
  get_remote_str $SCRIPT_URL
  LATEST_VERSION=$(bash -c "$ret" -- -v)

  compare_versions $LATEST_VERSION $VERSION

  # $LATEST_VERSION is newer $VERSION (the version installed)
  if [[ $? == 1 ]]; then
    echo -e "A new version of Simple 2D is available.\n"
    prompt_to_continue "Upgrade from v$VERSION to v$LATEST_VERSION?"
    update_check_sdl
    install_s2d $LATEST_VERSION
    success_msg "Simple 2D has been updated to $LATEST_VERSION!"
    echo -e "View the release notes:
${UNDERLINE}https://github.com/simple2d/simple2d/releases/latest${NORMAL}"; echo

  # $LATEST_VERSION is the same as installed version
  else
    echo "Simple 2D is already up to date!"
  fi
}


# Doctor #######################################################################


# Runs the diagnostics
doctor() {

  echo; echo -e "Running diagnostics...\n"

  errors=false

  if ! have_sdl2_libs?; then
    errors=true
    echo; error_msg "SDL libraries missing"
  fi

  if have_lib? 'simple2d'; then
    echo
  else
    errors=true
    echo; error_msg "Simple 2D library missing"
  fi

  if $errors; then
    echo -e "Issues were found.\n"
  else
    success_msg "No issues found!"
  fi
}


# Simulator ####################################################################


# Get list of booted iOS or tvOS simulators, store in $booted
#   $1  Equals 'cmd' if called from command line
simulator_booted() {
  booted=`xcrun simctl list devices | grep Booted`
  if [[ $1 == 'cmd' ]]; then
    if [[ $booted == '' ]]; then
      echo "No devices are booted."
    else
      echo $booted
    fi
  fi
}


# List all iOS and tvOS simulator devices available
simulator_list() {
  xcrun simctl list devices | grep -iv unavailable
}


# Open an iOS or tvOS simulator
#   $1  The device name, e.g. iPhone 7
# See `simulator_list` function for available names
simulator_open() {
  device=$1

  # Check if device name exists
  if ! xcrun simctl list devices | grep -q "$device"; then
    error_msg "\"$device\" does not match any simulator device names"
    echo -e "Choose a device name from the following:\n"
    simulator_list
    exit 1
  fi

  # Get the device UDID
  device_udid=`xcrun simctl list devices | grep "$device" | head -n1 | egrep -wo '\([-0-9A-F]+\)' | tr -d '\(\)'`
  echo "Requested device: $device with UDID $device_udid"
  simulator_booted

  # If the current simulator running is not the requested device, then quit
  if [[ $booted != '' && $booted != *$device* ]]; then
    echo "Quitting current device simulator..."
    osascript -e 'quit app "Simulator"'
  fi

  # Open the requsted simulator
  echo "Opening $device simulator..."
  open -a Simulator --args -CurrentDeviceUDID $device_udid

  # Wait for device to boot
  not_booted=true
  while $not_booted; do
    simulator_booted
    if [[ $booted == *$device* ]]; then
      not_booted=false
    else
      sleep 2
    fi
  done
}


# Install an app on the booted iOS or tvOS simulator
#   $1  The path to the `.app` file, e.g. Release-iphonesimulator/MyApp.app
simulator_install() {
  echo "Installing app..."
  xcrun simctl install booted $1
}


# Launch an installed app on the booted iOS or tvOS simulator
#   $1  The bundle identifier, e.g. "Simple2D.MyApp"
simulator_launch() {
  echo "Launching app..."
  xcrun simctl launch booted $1
}


# Stream log of currently running iOS or tvOS simulator
#   $1  Optional app name to filter by
simulator_log() {
  if [[ $1 == '' ]]; then
    xcrun simctl spawn booted log stream --level=debug
  else
    xcrun simctl spawn booted log stream --predicate "processImagePath endswith \"$1\""
  fi
}


# Stream log of currently running iOS or tvOS simulator, error messages only
simulator_log_errors() {
  xcrun simctl spawn booted log stream --predicate 'eventMessage contains "error" or messageType == error'
}


# Platform Specifics ###########################################################


# Prints homebrew message and quit
# params:
#   $1  String  The message to show, e.g. '--sdl'
macos_homebrew_message() {

  if [[ $1 == '--sdl' ]]; then
    echo -e "
We recommend using ${BOLD}Homebrew${NORMAL} to install, update, and uninstall
SDL on macOS. If you installed Simple 2D using Homebrew, the
following SDL packages were installed:

  sdl2
  sdl2_image
  sdl2_mixer
  sdl2_ttf

Run \`brew info <formula>\` to see information about each and
their dependencies, which were also installed.

Learn more at ${UNDERLINE}http://brew.sh${NORMAL}
"
  elif [[ $1 == '--HEAD' ]]; then
    echo -e "
Use ${BOLD}Homebrew${NORMAL} to install, update, and uninstall Simple 2D on macOS.

  To install the latest changes from the \`master\` development branch, use:
    brew install --HEAD simple2d

Note \`brew update\` will not update formulas installed with \`--HEAD\`,
but you can use \`brew reinstall --HEAD simple2d\` to manually grab
the latest changes.
"
  else
    echo -e "
Use ${BOLD}Homebrew${NORMAL} to install, update, and uninstall Simple 2D on macOS.

  First, use \`brew tap\` to get Simple 2D formulas:
    brew tap simple2d/tap

  Then, the following \`brew\` commands will be available:
    brew install simple2d
    brew upgrade simple2d
    brew uninstall simple2d

Learn more at ${UNDERLINE}http://brew.sh${NORMAL}
"
  fi

  exit
}


# Commands only available on macOS
macos_only_command() {
  echo -e "This command is only available on macOS"; exit 1
}


# For stuff not yet implemented in MinGW
mingw_not_implemented_message() {
  echo -e "This isn't implemented in MinGW yet :("; exit 1
}


# Detect Platform ##############################################################


unamestr=$(uname)

# macOS
if [[ $unamestr == 'Darwin' ]]; then
  platform_display='macOS'
  platform='macos'

# ARM
elif [[ $(uname -m) =~ 'arm' && $unamestr == 'Linux' ]]; then
  platform_display='ARM / Linux'
  platform='arm'

  if [[ $(cat /etc/os-release | grep -i raspbian) ]]; then
    platform_display='ARM / Linux (Raspberry Pi)'
    platform_rpi=true
  fi

# Linux
elif [[ $unamestr == 'Linux' ]]; then
  platform_display='Linux'
  platform='linux'

# Windows / MinGW
elif [[ $unamestr =~ 'MINGW' ]]; then
  platform_display='Windows / MinGW'
  platform='mingw'
fi

# Unsupported platform
if [[ $platform == 'unknown' ]]; then
  echo; error_msg "Not a supported system (macOS, Linux, or ARM platform)"; exit 1
fi


# Check Command-line Arguments #################################################


print_usage() {
echo -e "${BOLD}Simple 2D is a simple, open-source 2D graphics engine for everyone.${NORMAL}

Usage: simple2d [--libs] [-v|--version]
                <command> <options>

Summary of commands and options:
  build         Build a C/C++ source file or Xcode project; run for options
  install       Installs the latest release
    --HEAD        Installs from the development branch
    --sdl         Installs SDL only
  uninstall     Removes Simple 2D files
    --sdl         Removes SDL only
  update        Updates to latest release
  doctor        Runs diagnostics, checks installation, reports issues
  simulator     Interact with iOS and tvOS simulators; run for options
  --libs        Outputs libraries needed to compile Simple 2D apps
  -v|--version  Prints the installed version
"
}


print_usage_build() {
  echo -e "
Use the ${BOLD}build${NORMAL} command to compile or build Simple 2D apps.

For compiling C/C++ source files, use:
  build <c_or_cpp_file>

To build an Xcode project, use:
  build <sdk_option> <optional_path_to_xcode_project>

For <sdk_option> above, use one of the following:
  --ios            iOS simulator SDK
  --ios-device     iOS device SDK (requires signing)
  --tvos           tvOS simulator SDK
  --tvos-device    tvOS device SDK (requires signing)
"
}


print_usage_simulator() {
echo -e "
Choose an option with the ${BOLD}simulator${NORMAL} command:

  --list      List available devices
  --booted    Show currently booted devices

  --open <device_name>    Open a simulator device with a given device name

  --install <app_file>    Install an app on the booted simulator given the path
                          to the app e.g. \"Release-iphonesimulator/MyApp.app\"

  --launch <bundle_id>    Launch an app given the app bundle's identifier,
                          e.g. \"Simple2D.MyApp\"

  --log           Stream log of the booted simulator
  --log <app>     Stream log for the app only, given the app name
  --log-errors    Stream log containing only error messages
"
}


case $1 in
  build)
    build $2 $3;;
  install)
    case $2 in
      '')
        install;;
      -y)
        confirmed=true
        install;;
      --HEAD)
        install '--HEAD';;
      --sdl)
        install '--sdl';;
      *)
        print_usage;;
    esac;;
  uninstall)
    case $2 in
      '')
        uninstall;;
      --sdl)
        uninstall '--sdl';;
      *)
        print_usage;;
    esac;;
  update)
    case $2 in
      '')
        update;;
      --HEAD)
        update '--HEAD';;
      *)
        print_usage;;
    esac;;
  doctor)
    doctor;;
  simulator)
    if [[ $platform != 'macos' ]]; then macos_only_command; fi
    case $2 in
      '')
        print_usage_simulator;;
      --list)
        simulator_list;;
      --booted)
        simulator_booted 'cmd';;
      --open)
        simulator_open "$3";;
      --install)
        simulator_install "$3";;
      --launch)
        simulator_launch "$3";;
      --log)
        simulator_log "$3";;
      --log-errors)
        simulator_log_errors;;
      *)
        print_usage_simulator;;
    esac;;
  --libs)
    if [[ $platform == 'macos' ]]; then
      FLAGS='-Wl,-framework,OpenGL'
    elif [[ $platform == 'linux' ]]; then
      FLAGS='-lGL -lm'
    elif [[ $platform == 'arm' ]]; then
      FLAGS='-lm -I/opt/vc/include/ -L/opt/vc/lib'
      if $platform_rpi; then
        FLAGS+=' -lbrcmGLESv2'
      else
        FLAGS+=' -lGLESv2'
      fi
    fi
    if [[ $platform == 'mingw' ]]; then
      echo "-I/usr/local/include/ -L/usr/local/lib -lmingw32 -lsimple2d -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lopengl32 -lglew32"
    else
      echo "-lsimple2d `sdl2-config --cflags --libs`"\
           "${FLAGS} -lSDL2_image -lSDL2_mixer -lSDL2_ttf"
    fi;;
  -v|--version)
    echo $VERSION;;
  *)
    print_usage;;
esac
