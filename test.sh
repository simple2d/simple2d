#!/bin/bash

BOLD='\033[1;39m'
BLUE='\033[1;34m'
SUCCESS='\033[1;32m'
NORMAL='\033[0m'

echo -e "\n${BLUE}==>${BOLD} make uninstall${NORMAL}\n" && \
make uninstall && \
echo -e "\n${BLUE}==>${BOLD} make clean${NORMAL}\n" && \
make clean && \
echo -e "\n${BLUE}==>${BOLD} make${NORMAL}\n" && \
make && \
echo -e "\n${BLUE}==>${BOLD} make install${NORMAL}\n" && \
make install && \
echo -e "\n${BLUE}==>${BOLD} make tests${NORMAL}\n" && \
make tests && \
echo -e "\n${BLUE}==>${SUCCESS} Build successful!${NORMAL}\n" && \
echo -e "${BLUE}==>${BOLD} Running ./tests/$1 ${NORMAL}\n" && \
./tests/$1
