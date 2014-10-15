#!/bin/sh

LDFLAGS=""
platform="unknown"
unamestr=$(uname)
if [[ "$unamestr" == 'Darwin' ]]; then
   platform='darwin'
elif [[ "$unamestr" == 'Linux' ]]; then
   platform='linux'
   LDFLAGS='-lGL'
fi

echo -lsimple2d `sdl2-config --static-libs` ${LDFLAGS} -lSDL2_image -lSDL2_mixer -lSDL2_ttf
