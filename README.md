# Simple 2D

Simple 2D is a small library written in C that provides basic 2D graphics functionality and access to input devices and sound. It uses [SDL2](http://www.libsdl.org) for cross-platform, low-level access to hardware.

## Compiling

First, install the [SDL2 libraries](http://www.libsdl.org/download-2.0.php). For OS X, simply use [Homebrew](http://brew.sh):

```bash
brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

Compile and install Simple 2D using `make && make install`, then build your app as follows:

```bash
cc app.c `simple2d-config`
```

## Examples

Check out the [examples](/examples) directory to learn more. Build the examples using:

```bash
make examples
```
