#!/bin/bash

# ------------------------------------------------------------------------------
# The Simple 2D Command-Line Utility
# 
# This script can:
#  - Install Simple 2D on OS X, Linux, and the Raspberry Pi (Raspbian)
#  - Update Simple 2D in place
#  - Provide libraries for compiling with applications
#  - Check for issues with Simple 2D and SDL
# 
# Run from the web using:
#   bash <(curl -fsSL http://script_url_here)
# or
#   bash <(wget -qO - http://script_url_here)
# ------------------------------------------------------------------------------

# Set Constants ################################################################

VERSION='0.0.0'
VERSION_URL='https://raw.githubusercontent.com/simple2d/simple2d/master/VERSION'

# Colors
BOLD='\033[1;39m'
BLUE='\033[1;34m'
RED='\033[1;31m'
INFO='\033[4;36m'    # cyan underline
WARN='\033[4;33m'    # yellow underline
ERROR='\033[4;31m'   # red underline
SUCCESS='\033[1;32m' # green bold
NORMAL='\033[0m'     # reset

# Set Variables ################################################################

platform='unknown'
platform_display='unknown'

install_edge=false

ret=''  # the return value used by some functions

# Helper Functions #############################################################

prompt_to_continue() {
  read -p "  $1 (y/n) " ans
  if [[ $ans != "y" ]]; then
    echo -e "\nQuitting...\n"
    exit
  fi
  echo
}

print_task() {
  printf "${BLUE}==>${BOLD} $1...${NORMAL}$2"
}

have_lib?() {
  print_task "Checking for $1"
  
  if $(ld -o /dev/null -l$1 2>&1 | grep -qE '(library not found|ld: cannot find)'); then
    echo -e " no${NORMAL}"
    return 1
  else
    echo -e " yes${NORMAL}"
    return 0
  fi
}

get_remote_str() {
  if which curl > /dev/null; then
    ret=$(curl -fsSL $1)
  else
    ret=$(wget -qO - $1)
  fi
}

start_timer() {
  START_TIME=$SECONDS
}

end_timer() {
  # Print the time elapsed
  ELAPSED_TIME=$(($SECONDS - $START_TIME))
  echo -e "${BOLD}Finished in $(($ELAPSED_TIME/60/60)) hr, $(($ELAPSED_TIME/60%60)) min, and $(($ELAPSED_TIME%60)) sec${NORMAL}\n"
}

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

# Installation #################################################################

# Check for SDL2 libraries
have_sdl2_libs?() {
  
  have_all_libs=true
  have_sdl2_lib=true
  have_image_lib=true
  have_mixer_lib=true
  have_ttf_lib=true
  
  if ! have_lib? 'SDL2'; then
    have_all_libs=false
    have_sdl2_lib=false
  fi
  
  if ! have_lib? 'SDL2_image'; then
    have_all_libs=false
    have_image_lib=false
  fi
  
  if ! have_lib? 'SDL2_mixer'; then
    have_all_libs=false
    have_mixer_lib=false
  fi
  
  if ! have_lib? 'SDL2_ttf'; then
    have_all_libs=false
    have_ttf_lib=false
  fi
  
  if $have_all_libs; then
    return 0
  else
    echo; error_msg "Missing SDL libraries."
    return 1
  fi
}

# Install SDL on OS X
install_sdl_osx() {
  
  if have_sdl2_libs?; then
    echo; info_msg "SDL already installed."
  else
    echo "Install SDL2 libraries using Homebrew:"
    echo "  brew update"
    printf "  brew install "
    
    if ! $have_sdl2_lib ; then
      printf "sdl2 "
    fi
    
    if ! $have_image_lib ; then
      printf "sdl2_image "
    fi
    
    if ! $have_mixer_lib ; then
      printf "sdl2_mixer "
    fi
    
    if ! $have_ttf_lib ; then
      printf "sdl2_ttf "
    fi
    
    echo -e "\n\nLearn more at http://brew.sh\n"
    exit
  fi
}

# Install SDL on Linux
install_sdl_linux() {
  
  if have_sdl2_libs?; then
    echo; info_msg "SDL already installed."
  else
    prompt_to_continue "Install SDL now?"
    
    print_task "Updating packages" "\n\n"
    sudo apt-get update
    echo; print_task "Installing SDL2" "\n\n"
    sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
    echo
    
    if ! have_sdl2_libs?; then
      error_msg "SDL libraries did not install correctly."
      exit
    else
      echo; info_msg "SDL was installed successfully."
    fi
  fi
}

# Install SDL on Raspberry Pi
install_sdl_rpi() {
  
  if have_sdl2_libs?; then
    echo; info_msg "SDL already installed."
  else
    warning_msg "Installing SDL can take up to an hour on the Raspberry Pi."
    prompt_to_continue "Install now?"
    
    # Install dependencies
    print_task "Installing SDL2 dependencies"
    sudo apt-get update
    sudo apt-get install -y libudev-dev libasound2-dev libdbus-1-dev  # for SDL2
    sudo apt-get install -y libfreetype6-dev  # for SDL2_ttf
    
    # Setting up variables
    url="http://www.libsdl.org"
    
    sdl="SDL2-2.0.3"
    sdl_url="${url}/release/${sdl}.tar.gz"
    
    image="SDL2_image-2.0.0"
    image_url="${url}/projects/SDL_image/release/${image}.tar.gz"
    
    mixer="SDL2_mixer-2.0.0"
    mixer_url="${url}/projects/SDL_mixer/release/${mixer}.tar.gz"
    
    ttf="SDL2_ttf-2.0.12"
    ttf_url="${url}/projects/SDL_ttf/release/${ttf}.tar.gz"
    
    install_sdl_lib () {
      cd /tmp
      wget -N $1
      tar -xzf $2.tar.gz
      cd $2
      ./configure && make && sudo make install
      rm /tmp/$2.tar.gz
      rm -rf /tmp/$2
    }
    
    if ! $have_sdl2_lib ; then
      echo; print_task "Installing SDL2" "\n\n"
      install_sdl_lib $sdl_url $sdl
    fi
    
    if ! $have_image_lib ; then
      echo; print_task "Installing SDL2_image" "\n\n"
      install_sdl_lib $image_url $image
    fi
    
    if ! $have_mixer_lib ; then
      echo; print_task "Installing SDL2_mixer" "\n\n"
      install_sdl_lib $mixer_url $mixer
    fi
    
    if ! $have_ttf_lib ; then
      echo; print_task "Installing SDL2_ttf" "\n\n"
      install_sdl_lib $ttf_url $ttf
    fi
    
    if ! have_sdl2_libs?; then
      error_msg "SDL libraries did not install correctly."
      exit
    else
      echo; info_msg "SDL was installed successfully."
    fi
  fi
}

# Install SDL
install_sdl() {
  
  print_task "Installing SDL" "\n\n"
  
  if [[ $platform == 'osx' ]]; then
    install_sdl_osx
  elif [[ $platform == 'linux' ]]; then
    install_sdl_linux
  elif [[ $platform == 'rpi' ]]; then
    install_sdl_rpi
  fi
}

# Uninstall SDL
uninstall_sdl() {
  
  echo; print_task "Uninstalling SDL" "\n\n"
  
  if [[ $platform == 'osx' ]]; then
    echo "If using Homebrew, use:"
    echo "  brew uninstall sdl2 sdl2_image sdl2_mixer sdl2_ttf"; echo
  elif [[ $platform == 'linux' ]]; then
    apt-get --purge remove libsdl2*
  elif [[ $platform == 'rpi' ]]; then
    # TODO: Implement this
    echo "Not yet implemented, sorry!"; echo
  fi
}

# Common Simple 2D installation steps
install_s2d() {
  
  mkdir /tmp/s2d
  
  if [[ $1 == 'master' ]]; then
    prefix=''
  else
    prefix='v'
  fi
  
  print_task "Downloading Simple 2D" "\n\n"
  # Linux and Raspberry Pi may not have curl installed by default
  if which curl > /dev/null; then
    curl -L https://github.com/simple2d/simple2d/archive/$prefix$1.zip -o /tmp/s2d/$prefix$1.zip
  else
    wget -NP /tmp/s2d https://github.com/simple2d/simple2d/archive/$prefix$1.zip
  fi
  
  # Check if archive was downloaded properly
  if [ ! -f "/tmp/s2d/$prefix$1.zip" ]; then
    echo; error_msg "Simple 2D could not be downloaded."
    exit
  fi
  
  echo; print_task "Unpacking"
  unzip -q /tmp/s2d/$prefix$1.zip -d /tmp/s2d
  
  # Check if archive was unpacked properly
  if [[ $? != 0 ]]; then
    echo; error_msg "The downloaded Simple 2D package is damaged. Could not unpack."
    exit
  fi
  
  printf " done"
  cd /tmp/s2d/simple2d-$1
  
  echo; print_task "Compiling" "\n\n"
  make
  
  echo; print_task "Installing" "\n\n"
  if [[ $platform == 'osx' ]]; then
    make install
  else
    sudo make install
  fi
  
  echo; print_task "Cleaning up"
  cd
  rm -rf /tmp/s2d
  echo -e " done"
  
  # Check if S2D installed correctly
  if ! have_lib? 'simple2d'; then
    echo; error_msg "Simple 2D did not install correctly."
    exit
  fi
  echo
}

# Install Simple 2D
install() {
  
  # Welcome message
  echo -e "\n${BOLD}Welcome to Simple 2D!${NORMAL}"
  echo -e "${BLUE}---------------------${NORMAL}\n"
  
  echo -e "Platform detected: ${BOLD}${platform_display}${NORMAL}\n"
  
  if have_lib? 'simple2d' > /dev/null; then
    warning_msg "Simple 2D is already installed. Proceeding will reinstall."
  fi
  
  echo -e "This will install Simple 2D to the following locations:
    /usr/local/include/simple2d.h
    /usr/local/lib/libsimple2d.a
    /usr/local/bin/simple2d"
  echo
  
  prompt_to_continue "Continue?"
  
  start_timer
  
  install_sdl
  
  if $install_edge; then
    install_s2d 'master'
  else
    install_s2d $VERSION
  fi
  
  end_timer
  
  success_msg "Simple 2D installed successfully!"
}

# Uninstall ####################################################################

uninstall() {
  
  echo; prompt_to_continue "Uninstall Simple 2D?"
  
  # Use hard-coded, absolute paths for safety
  rm -f /usr/local/include/simple2d.h
  rm -f /usr/local/lib/libsimple2d.a
  rm -f /usr/local/bin/simple2d
  
  # Check if files were actually deleted
  files=(/usr/local/include/simple2d.h
         /usr/local/lib/libsimple2d.a
         /usr/local/bin/simple2d)
  
  if [ -f ${files[0]} -o -f ${files[1]} -o -f ${files[2]} ]; then
    echo; error_msg "Simple 2D files could not be removed. Try using \`sudo\`?"
  else
    echo; success_msg "Simple 2D uninstalled."
  fi
}

# Update #######################################################################

update() {
  
  # Is Simple 2D installed?
  echo
  if ! have_lib? 'simple2d' > /dev/null; then
    error_msg "Simple 2D isn't currently installed."
    echo -e "Use the \`install\` command to install it.\n"
    exit
  fi
  
  # From: stackoverflow.com/questions/4023830/bash-how-compare-two-strings-in-version-format
  compare_versions () {
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
  
  update_check_sdl() {
    if have_sdl2_libs?; then
      echo; info_msg "SDL already installed."
    else
      echo -e "Run \`simple2d doctor\` for more information, or..."
      echo -e "Run \`simple2d install --sdl\` to install missing libraries.\n"
      exit
    fi
  }
  
  if $install_edge; then
    echo -e "This will update Simple 2D to the bleeding edge."
    prompt_to_continue "Continue?"
    update_check_sdl
    install_s2d 'master'
    success_msg "Updated Simple 2D to latest commit!"
    echo "  Go to:"
    echo "    http://github.com/simple2d/simple2d/commits/master"
    echo "  to see the revision history."; echo
  else
    get_remote_str $VERSION_URL
    NEW_VERSION=$ret
    
    compare_versions $NEW_VERSION $VERSION
    
    if [[ $? == 1 ]]; then
      echo -e "A new version of Simple 2D is available."
      prompt_to_continue "Install now?"
      
      update_check_sdl
      
      print_task "Updating Simple 2D" "\n"
      install_s2d $NEW_VERSION
      
      success_msg "Simple 2D has been updated to $NEW_VERSION!"
    else
      success_msg "Simple 2D is up to date."
    fi
  fi
}

# Doctor #######################################################################

doctor() {
  
  echo; print_task "Running diagnostics" "\n\n"
  
  errors=false
  
  if ! have_sdl2_libs?; then
    errors=true
  fi
  
  if have_lib? 'simple2d'; then
    echo
  else
    errors=true
    echo; error_msg "Simple 2D library missing."
  fi
  
  if $errors; then
    echo -e "Issues found.\n"
  else
    success_msg "No issues found!"    
  fi
}

# Detect Platform ##############################################################

unamestr=$(uname)
if [[ $unamestr == 'Darwin' ]]; then
  platform_display='Mac OS X'
  platform='osx'
elif [[ $unamestr == 'Linux' ]]; then
  platform_display='Linux'
  platform='linux'
elif [[ $(uname -m) == 'armv6l' && $unamestr == 'Linux' ]]; then
  platform_display='Raspberry Pi'
  platform='rpi'
fi

# Look for supported platform
if [[ $platform == 'unknown' ]]; then
  echo; error_msg "Not a supported system (OS X, Linux, Raspberry Pi)."
  exit
fi

# Check Command-line Arguments #################################################

print_usage() {
echo "
Usage: simple2d [-l|--libs] [-v|--version]
                <command> <args>

Summary of commands and options:
  install       Installs the latest stable version.
    --edge      Installs latest build (possibly unstable).
    --sdl       Installs SDL only.
  uninstall     Removes Simple 2D files only.
    --sdl       Removes SDL only.
  update        Updates to latest stable version.
    --edge      Updates to latest build (possibly unstable).
  doctor        Checks the installation status, reports issues.
  -l|--libs     Outputs libraries needed to compile Simple 2D apps.
  -v|--version  Prints the installed version.
"
}

case $1 in
  install)
    case $2 in
      '')
        install;;
      --edge)
        install_edge=true
        install;;
      --sdl)
        echo; install_sdl;;
      *)
        print_usage;;
    esac;;
  uninstall)
    case $2 in
      '')
        uninstall;;
      --sdl)
        uninstall_sdl;;
      *)
        print_usage;;
    esac;;
  update)
    case $2 in
      '')
        update;;
      --edge)
        install_edge=true
        update;;
      *)
        print_usage;;
    esac;;
  doctor)
    doctor;;
  -l|--libs)
    if [[ $platform == 'linux' ]]; then
      LDFLAGS='-lGL'
    fi
    echo "-lsimple2d `sdl2-config --static-libs` ${LDFLAGS} -lSDL2_image -lSDL2_mixer -lSDL2_ttf";;
  -v|--version)
    echo $VERSION;;
  *)
    print_usage;;
esac
