# Welcome to Simple 2D!

Simple 2D is a small, open-source graphics engine providing essential 2D drawing, media, and input capabilities. It's written in C and works across many platforms, creating native windows and interacting with hardware using [SDL](http://www.libsdl.org) while rendering content with [OpenGL](https://www.opengl.org).

Note that this README will be continuously updated as new features are added, bugs are fixed, and other changes are made. [View the release notes](https://github.com/simple2d/simple2d/releases) for a link to that version's documentation.

If you encounter any issues, ping the [mailing list](https://groups.google.com/d/forum/simple2d). Learn about [contributing](#contributing) below.

## Getting Started

Simple 2D supports Unix-like systems and is tested on the latest versions of OS X, Ubuntu, and Raspbian on the Raspberry Pi.

To install the [latest release](https://github.com/simple2d/simple2d/releases/latest)...

### ...on macOS

Use [Homebrew](http://brew.sh):

```bash
brew tap simple2d/tap
brew install simple2d
```



### ...on Linux

Run the [`simple2d.sh`](bin/simple2d.sh) Bash script. Everything will be explained along the way and you'll be prompted before any action is taken. To run this script from the web, copy and paste this snippet in your terminal:

```bash
url='https://raw.githubusercontent.com/simple2d/simple2d/master/bin/simple2d.sh'; which curl > /dev/null && cmd='curl -fsSL' || cmd='wget -qO -'; bash <($cmd $url) install
```

#### A note on Linux/ARM platforms

Simple 2D supports ARM platforms running Linux, like the [Raspberry Pi](https://www.raspberrypi.org) and [CHIP](https://nextthing.co). Since most Linux distributions have SDL packages configured for traditional desktop platforms, the install script will compile SDL from source when ARM is detected, disabling windowing systems (like X11) and OpenGL (forcing OpenGL ES).

### The Command-line Utility

Once installed, use the `simple2d` command-line utility to update Simple 2D, check for issues, output the libraries needed to compile applications, and more. Run `simple2d --help` to see all available commands and options.

## Building from Source

Alternatively, you can compile and install Simple 2D from source. First clone the repo using:

```bash
git clone --recursive https://github.com/simple2d/simple2d.git
```

To keep the size of this repository small, [git submodules](https://git-scm.com/book/en/Git-Tools-Submodules) are used to reference test media and Windows dependencies. The `--recursive` flag ensures submodules are initialize and updated when this repo is cloned. If you happened to clone this repo without the `--recursive` flag, you can still initialize and update submodules with:

```bash
git submodule init
git submodule update --remote
```

Update these submodules at any time using `git submodule update --remote`

On Unix-like systems, including Windows using MinGW, run:

```bash
make && make install
```

On Windows using Visual C++, run:

```cmd
nmake /f NMakefile all install
```

Note, on macOS and Linux, the makefile will not check for or install dependencies, unlike installing via Homebrew or the `simple2d.sh` script, respectively. Dependencies for Windows, supporting both Visual C++ and MinGW, _are_ included in this repo (referenced by the [`windows-deps`](https://github.com/simple2d/windows-deps) submodule) and installed by both makefiles.

On Windows using Visual C++, Simple 2D will be installed to `%LOCALAPPDATA%\simple2d`, so make sure to add that to your path (for example with `set PATH=%PATH%;%LOCALAPPDATA%\simple2d`). In all other cases, it will be installed to `/usr/local/`. On Windows using MinGW, make sure to add `/usr/local/bin` to your path.

## Tests

Simple 2D has a few test programs to make sure all functionality is working as it should.

- [`auto.c`](tests/auto.c) – A set of automated unit tests for the public interface.
- [`triangle.c`](tests/triangle.c) – The "Hello Triangle" example in this README.
- [`testcard.c`](tests/testcard.c) – A graphical card, similar to [TV test cards](https://en.wikipedia.org/wiki/Test_card), with the goal of ensuring all visuals and inputs are working properly.
- [`audio.c`](tests/audio.c) – Tests audio functions with various file formats interpreted as sound samples and music.

### Building and Running Tests

Run `make tests`, or `nmake /f NMakefile tests` on Windows using Visual C++, to compile tests to the `tests/` directory, which will have the same name as their C source file. Since media paths are set relatively in these test programs, make sure to `cd` into the `tests/` directory before running a test, for example:

```bash
# on Unix-like systems
make tests && cd tests/ && ./testcard

# on Windows using MinGW
make tests & cd tests\ & testcard.exe

# on Windows using Visual C++
nmake /f NMakefile tests & cd tests\ & testcard.exe
```

Each test also has a makefile target, so you can build and run tests using, for example, `make tests testcard`. Or, conveniently uninstall everything, rebuild Simple 2D and tests from source, and run tests using `make rebuild <name_of_test>`, for example:

```bash
# rebuild and run `auto.c` then `testcard.c`

# on Unix-like systems and Windows using MinGW
make rebuild auto testcard

# on Windows using Visual C++
nmake /f NMakefile rebuild auto testcard
```

---

# Creating Apps with Simple 2D

Making 2D apps is simple! Let's create a window and draw a triangle...

```c
#include <simple2d.h>

void render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}

int main() {
  
  S2D_Window *window = S2D_CreateWindow(
    "Hello Triangle", 640, 480, NULL, render, 0
  );
  
  S2D_Show(window);
  S2D_FreeWindow(window);
  return 0;
}
```

Save the code above to a file called `triangle.c`, and compile by running `simple2d build triangle.c` on the command line (in MinGW, run this in a Bash prompt). Then run the app using `./triangle` on macOS and Linux, or `triangle.exe` on Windows. Finally, enjoy your stunning triangle in a 640x480 window at 60 frames per second!

The `simple2d build` command is a helpful shortcut for compiling a single source file. Of course, you can use a compiler directly, for example on Unix-like systems:

```bash
cc triangle.c `simple2d --libs` -o triangle
```

And on Windows using Visual C++ in a developer command prompt:

```bash
cl triangle.c /I %LOCALAPPDATA%\simple2d /link /LIBPATH %LOCALAPPDATA%\simple2d\simple2d.lib /SUBSYSTEM:CONSOLE

# as a PowerShell command
iex "cl triangle.c $(simple2d --libs)"
```

## 2D Basics

Let's learn more about structuring applications for 2D drawing and more.

### The Window

All rendered content, input, and sound is controlled by the window, and so creating a window is the first thing you'll do. Start by declaring a pointer to a `Window` structure and initializing it using `S2D_CreateWindow`.

```c
S2D_Window *window = S2D_CreateWindow(
  "Hello World!",  // title of the window
  800, 600,        // width and height
  update, render,  // callback function pointers (these can be NULL)
  0                // flags
);
```

The window flags can be `0` or any one of the following:

```c
S2D_RESIZABLE
S2D_BORDERLESS
S2D_FULLSCREEN
S2D_HIGHDPI
```

Flags can also be combined using the bitwise OR operator, for example: `S2D_RESIZABLE | S2D_BORDERLESS`.

The viewport can also be set independently of the window size, for example:

```c
window->viewport.width  = 400;
window->viewport.height = 300;
```

The viewport has various scaling modes, such as `S2D_FIXED` (viewport stays the same size as window size changes), `S2D_SCALE` (the default, where the viewport scales proportionately and is centered in the window), or `S2D_STRETCH` (viewport stretches to fill the entire window). Set the mode like so:

```c
window->viewport.mode = S2D_FIXED;
```

Before showing the window, this attribute can be set:

```c
window->vsync = false;  // set the vertical sync, true by default
```

Once your window is ready to go, it can be shown using:

```c
S2D_Show(window);
```

Anytime before or during the window is being shown, these attributes can be set:

```c
// Cap the frame rate, 60 frames per second by default
window->fps_cap = 30;

// Set the window background color, black by default
window->background.r = 1.0;
window->background.g = 0.5;
window->background.b = 0.8;
window->background.a = 1.0;
```

Callback functions can also be changed anytime – more on that below. Many values can also be read from the `Window` structure, see the [`simple2d.h`](include/simple2d.h) header file for details.

When you're done with the window, free it using:

```c
S2D_FreeWindow(window);
```

### Update and Render

The window loop is where all the action takes place: the frame rate is set, input is handled, the app state is updated, and visuals are rendered. You'll want to declare two essential functions which will be called by the window loop: `update` and `render`. Like a traditional game loop, `update` is used for updating the application state, and `render` is used for drawing the scene. Simple 2D optimizes both functions for performance and accuracy, so it's good practice to keep those updating and rendering tasks separate.

The update and render functions should look like this:

```c
void update() { /* update your application state */ }
void render() { /* draw stuff */ }
```

Remember to add these function names when calling `S2D_CreateWindow` (see ["The Window"](#the-window) section above for an example).

To exit the window loop at anytime, call the following function:

```c
S2D_Close(window);
```

## Drawing Basics

Where a vertex is present, like with shapes, there will be six values which need to be set for each: the `x` and `y` coordinates, and four color values. All values are floats, although `x` and `y` coordinates are typically expressed as whole numbers (from 0 to whatever). When vertices have different color values, the space between them are blended as a gradient.

The shorthand for the examples below are:

```c
x = x coordinate
y = y coordinate

w = width
h = height

// Color range is from 0.0 to 1.0
r = red
g = green
b = blue
a = alpha
```

So, for example, `x2` would be the second `x` coordinate, and `b2` would be the blue value at that vertex.

### Shapes

There are two fundamental shapes available: triangles and quadrilaterals. Triangles are drawn with the function `S2D_DrawTriangle`:

```c
S2D_DrawTriangle(x1, y1, r1, g1, b1, a1,
                 x2, y2, r2, g2, b2, a2,
                 x3, y3, r3, g3, b3, a3);
```

Quadrilaterals are drawn with `S2D_DrawQuad`:

```c
S2D_DrawQuad(x1, y1, r1, g1, b1, a1,
             x2, y2, r2, g2, b2, a2,
             x3, y3, r3, g3, b3, a3,
             x4, y4, r4, g4, b4, a4);
```

### Images

Images in many popular formats, like JPEG, PNG, and BMP can also be drawn in the window. Unlike shapes, images need to be read from files and stored in memory. Simply declare a pointer to an `S2D_Image` structure and initialize it using `S2D_CreateImage`, giving it the file path to the image:

```c
S2D_Image *img = S2D_CreateImage("image.png");
```

If the image can't be found, `S2D_CreateImage` will return `NULL`.

Once you have your image, you can then change its `x, y` position like so:

```c
img->x = 125;
img->y = 350;
```

Change the size of the image by adjusting its width and height:

```c
img->w = 256;
img->h = 512;
```

You can also adjust the color of the image like this:

```c
// default is 1.0 for each, a white color filter
img->color.r = 1.0;
img->color.g = 0.8;
img->color.b = 0.2;
img->color.a = 1.0;
```

Finally, draw the image using:

```c
S2D_DrawImage(img);
```

Since the image was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeImage(img);
```

### Sprites

Sprites are special kinds of images which can be used to create animations. To create a sprite, declare a pointer to an `S2D_Sprite` structure and initialize it with `S2D_CreateSprite` providing the file path to the sprite sheet image.

```c
S2D_Sprite spr = S2D_CreateSprite("sprite_sheet.png");
```

If the sprite image can't be found, `S2D_CreateSprite` will return `NULL`.

Clip the sprite sheet to a single image using `S2D_ClipSprite` and provide a clipping rectangle:

```c
S2D_ClipSprite(spr, x, y, width, height);
```

The `x, y` position of the sprite itself can be changed like so:

```c
spr->x = 150;
spr->y = 275;
```

You can also adjust the color of the sprite image like this:

```c
// default is 1.0 for each, a white color filter
spr->img->color.r = 1.0;
spr->img->color.g = 0.8;
spr->img->color.b = 0.2;
spr->img->color.a = 1.0;
```

Finally, draw the sprite using:

```c
S2D_DrawSprite(spr);
```

Since the sprite was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeSprite(spr);
```

### Text

Text is drawn much like images. Start by finding your favorite OpenType font (with a `.ttf` or `.otf` file extension), then declare a pointer to a `S2D_Text` structure, and initialize it using `S2D_CreateText`, giving it the file path to the font, the message to display, and the size:

```c
S2D_Text *txt = S2D_CreateText("vera.ttf", "Hello world!", 20);
```

You can then change the `x, y` position of the text, for example:

```c
txt->x = 127;
txt->y = 740;
```

Change the color of the text like this:

```c
// default is 1.0 for each
txt->color.r = 0.5;
txt->color.g = 1.0;
txt->color.b = 0.0;
txt->color.a = 0.7;
```

Draw the text using:

```c
S2D_DrawText(txt);
```

You can also change the text message at any time. Use `S2D_SetText` and give it the `Text` pointer along with the new message:

```c
S2D_SetText(txt, "A different message!");
```

Since the text was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeText(txt);
```

## Input

Simple 2D can capture input from just about anything. Let's learn how to grab input events from the mouse, keyboard, and game controllers.

### Mouse

The cursor position of the mouse or trackpad can be read at any time from the window. Note that the top, left corner is the origin, `(0, 0)`.

```c
window->mouse.x;
window->mouse.y;
```

To capture mouse button presses, first define the `on_mouse` function:

```c
void on_mouse(int x, int y) {
  // mouse clicked at x, y
}
```

Then attach the callback to the window:

```c
window->on_mouse = on_mouse;
```

### Keyboard

There are two types of keyboard events captured by the window: a single key press and a key held down continuously. When a key is pressed, the window calls its `on_key` function once, and if the key is being held down, the `on_key_down` function will be repeatedly called with each cycle of the window loop.

To start capturing keyboard input, first define the `on_key` and `on_key_down` functions:

```c
// Do something with `key` in each of these functions
void on_key(const char *key) { ... }
void on_key_down(const char *key) { ... }
```

Then attach the callbacks to the window:

```c
window->on_key = on_key;
window->on_key_down = on_key_down;
```

### Game Controllers and Joysticks

There are two types of controller or joystick events captured by the window: axis motion and button presses. When a button is pressed or a joystick moved, the window calls its `on_controller` function once.

To start capturing game controller or joystick input, first define the `on_controller` function:

```c
// Do something with `axis` and `val` if `is_axis` is true
//   or...
// Do something with `btn` if `is_btn` is true
//
// Multiple controllers or joysticks may be distinguished by the ID number in `which`
void on_controller(int which, bool is_axis, int axis, int val, bool is_btn, int btn) { ... }
```

Then attach the callback to the window:

```c
window->on_controller = on_controller;
```

A [community-sourced database](https://github.com/gabomdq/SDL_GameControllerDB) of game controller mappings can be used to map numeric button and axis identifiers to named buttons and axes.

## Audio

Simple 2D supports a number of audio formats, including WAV, MP3, Ogg Vorbis, and FLAC. There are two kinds of audio concepts: sounds and music. Sounds are intended to be short samples, played without interruption. Music is for longer pieces which can be played, paused, stopped, resumed, and faded out.

### Sounds

Create a sound by first declaring a pointer to a `S2D_Sound` structure and initialize it using `S2D_CreateSound` and providing the path to the audio file:

```c
S2D_Sound *snd = S2D_CreateSound("sound.wav");
```

Then play the sound like this:

```c
S2D_PlaySound(snd);
```

Since sounds are allocated dynamically, free them using:

```c
S2D_FreeSound(snd);
```

### Music

Similarly, to create some music, declare a pointer to a `S2D_Music` structure and initialize it using `S2D_CreateMusic` providing the path to the audio file:

```c
S2D_Music *mus = S2D_CreateMusic("music.ogg");
```

Play the music using `S2D_PlayMusic` providing the pointer and the number of times to be repeated. For example, to play the music once, set the second parameter to `0`, and to repeat forever, set it to `-1`.

```c
S2D_PlayMusic(mus, -1);
```

Only one piece of music can be played at a time. The following functions for pausing, resuming, stopping, and fading out apply to whatever music is currently playing:

```c
S2D_PauseMusic();
S2D_ResumeMusic();
S2D_StopMusic();

// Fade out duration in milliseconds
S2D_FadeOutMusic(2000);
```

Since music is allocated dynamically, free it using:

```c
S2D_FreeMusic(mus);
```

# Contributing

> "Simple can be harder than complex: You have to work hard to get your thinking clean to make it simple. But it's worth it in the end because once you get there, you can move mountains." ― [Steve Jobs](http://blogs.wsj.com/digits/2011/08/24/steve-jobss-best-quotes)

Despite the continuing advancement of graphics hardware and software, getting started with simple graphics programming isn't that easy or accessible. We're working to change that.

Check out the [open issues](https://github.com/simple2d/simple2d/issues) and join the [mailing list](https://groups.google.com/d/forum/simple2d). If you're a hardcore C and OS hacker, you should seriously consider contributing to [SDL](https://www.libsdl.org) so we can continue writing games without worrying about the platforms underneath. Take a look at the talks from [Steam Dev Days](http://www.steamdevdays.com), especially [Ryan C. Gordon's](https://twitter.com/icculus) talk on [Game Development with SDL 2.0](https://www.youtube.com/watch?v=MeMPCSqQ-34&list=UUStZs-X5W6V3TFJLnwkzN5w).

## Preparing a Release

1. [Run tests](#tests) on all supported platforms
2. Update documentation to reflect the current API
3. Update the version number in [`simple2d.sh`](bin/simple2d.sh) and [`simple2d.cmd`](bin/simple2d.cmd), commit changes
4. Create a [new release](https://github.com/simple2d/simple2d/releases) in GitHub, with tag in the form `v#.#.#`
5. Update the [Homebrew tap](https://github.com/simple2d/homebrew-tap):
  - Update formula with new release archive `url`
  - Run `brew audit --strict ./simple2d.rb` to detect any issues
  - Calculate the new `sha256` using `brew install ./simple2d.rb` (note the "SHA256 mismatch" error, use the "Actual" value)
  - Test installation using the same `brew install` command above
  - Commit changes to the formula
6. 🎉

# About the Project

Simple 2D was created by [Tom Black](https://twitter.com/blacktm), who thought simple graphics programming was way too difficult and decided to do something about it.

Everything is [MIT Licensed](LICENSE.md), so hack away.

Hope you enjoy this project!
