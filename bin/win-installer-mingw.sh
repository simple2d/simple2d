#!/bin/bash

# Colors
BOLD='\033[1;39m'     # default bold
SUCCESS='\033[1;32m'  # green   bold
NORMAL='\033[0m'      # reset

# Prompts to continue, or exits
# params:
#   $1  String  Message before y/n prompt
prompt_to_continue() {
  printf "${BOLD}$1${NORMAL} "
  read -p "(y/n) " ans
  if [[ $ans != "y" ]]; then
    echo -e "\nQuitting...\n"
    exit
  fi
  echo
}

# Prints and runs a command
# params:
#   $1  String  The command
print_and_run() {
  echo "==> $1"; $1
}

unamestr=$(uname)

# Look for MinGW
if [[ $unamestr =~ 'MINGW' ]]; then
  echo -e "${BOLD}MinGW detected!${NORMAL}"; echo
else
  echo -e "${BOLD}MinGW not detected!${NORMAL}"
  echo "This script is for installing Simple 2D on Windows with MinGW."
  echo "If you have MinGW, maybe environment variables aren't set?"
  exit
fi

echo "Simple 2D and its dependencies will be installed to the following locations:
  /usr/local/include
  /usr/local/lib
  /usr/local/bin"
echo

prompt_to_continue "Continue?"

print_and_run "mkdir -p /usr/local/include/"
print_and_run "mkdir -p /usr/local/lib/"
print_and_run "mkdir -p /usr/local/bin/"
print_and_run "cp -R include/*  /usr/local/include"
print_and_run "cp -R lib/*      /usr/local/lib"
print_and_run "cp -R bin/*      /usr/local/bin"

echo -e "\n${SUCCESS}Simple 2D installed successfully!${NORMAL}\n"
