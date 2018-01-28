# Welcome to Simple 2D!

Simple 2D is a small, open-source graphics engine providing essential 2D drawing, media, and input capabilities. It's written in C and works across many platforms, creating native windows and interacting with hardware using [SDL](https://www.libsdl.org) while rendering content with [OpenGL](https://www.opengl.org).

Please note this README will be continuously updated as new features are added, bugs are fixed, and other changes are made. [View the release notes](https://github.com/simple2d/simple2d/releases) for a link to that version's documentation.

If you encounter any issues, ping the [mailing list](https://groups.google.com/d/forum/simple2d). Learn about [contributing](#contributing) below.

## Getting started

Simple 2D supports all major operating systems and hardware platforms, and is tested on the latest releases of macOS, iOS, tvOS, Windows, Ubuntu, and Raspbian (on the Raspberry Pi).

To install the [latest release](https://github.com/simple2d/simple2d/releases/latest)...

### ...on macOS

Use [Homebrew](http://brew.sh):

```bash
brew tap simple2d/tap
brew install simple2d
```

#### iOS and tvOS

The Homebrew formula above will also install the iOS and tvOS frameworks to `/usr/local/Frameworks/Simple2D` by default. After installing, run the `simple2d simulator` command to see available options for interacting with the iOS and tvOS simulators. Run `simple2d build` to learn how to build Xcode projects with the iOS and tvOS SDKs. Example Xcode projects can be found in the [`deps` repository](https://github.com/simple2d/deps/tree/master/xcode).

### ...on Windows

[Download the Windows installer](https://github.com/simple2d/simple2d/releases/latest) for Visual C++ or MinGW.

For MinGW, we recommend using an [MSYS2](http://www.msys2.org) environment (also available on [Chocolatey](https://chocolatey.org/packages/msys2)). Simple 2D can also be installed on MinGW using the Linux instructions below.

### ...on Linux

Run the [`simple2d.sh`](bin/simple2d.sh) Bash script. Everything will be explained along the way and you'll be prompted before any action is taken. To run the script from the web, paste this snippet in your terminal:

```bash
url='https://raw.githubusercontent.com/simple2d/simple2d/master/bin/simple2d.sh'; which curl > /dev/null && cmd='curl -fsSL' || cmd='wget -qO -'; bash <($cmd $url) install
```

#### Linux/ARM platforms

Simple 2D supports ARM platforms running Linux, like the [Raspberry Pi](https://www.raspberrypi.org) and [CHIP](https://getchip.com). Since most Linux distributions have SDL packages configured for traditional desktop platforms, the install script will compile SDL from source when ARM is detected, disabling windowing systems (like X11) and OpenGL (forcing OpenGL ES).

### The command-line utility

Once installed, use the `simple2d` command-line utility to update Simple 2D, check for issues, output the libraries needed to compile applications, and more. Simply run `simple2d` to see all available commands and options.

## Building from source

Alternatively, you can compile and install Simple 2D from source. First clone this repo using:

```bash
git clone --recursive https://github.com/simple2d/simple2d.git
```

To keep the size of this repository small, [Git submodules](https://git-scm.com/book/en/Git-Tools-Submodules) are used to reference [test media](https://github.com/simple2d/test_media) and [dependencies](https://github.com/simple2d/deps). The `--recursive` flag ensures submodules are initialize and updated when this repo is cloned. If you happened to clone this repo without the `--recursive` flag, you can still initialize and update submodules with:

```bash
git submodule init
git submodule update --remote
```

Update these submodules at any time using `git submodule update --remote`

Next, build and install on Unix-like systems, including Windows using MinGW, by running:

```bash
make && make install
```

On Windows using Visual C++, open a 64-bit Visual Studio command prompt and run:

```cmd
nmake /f NMakefile all install
```

Note that on macOS and Linux, the makefile will not check for or install dependencies, unlike installing via Homebrew or the `simple2d.sh` script, respectively. Dependencies for Windows, supporting both Visual C++ and MinGW, _are_ included in this repo (referenced by the [`deps`](https://github.com/simple2d/deps) submodule) and installed by both makefiles.

On Windows using Visual C++, Simple 2D will be installed to `%LOCALAPPDATA%\simple2d`, so make sure to add that to your path (for example with `set PATH=%PATH%;%LOCALAPPDATA%\simple2d`). In all other cases, it will be installed to `/usr/local/`. On Windows using MinGW, make sure to add `/usr/local/bin` to your path as well.

### Building release archives

To build the release archives, which are attached as [downloads with each release](https://github.com/simple2d/simple2d/releases/latest), run `make release` on macOS and Windows using MinGW, and `nmake /f NMakefile release` on Windows using Visual C++.

## Tests

Simple 2D has a few test programs to make sure everything is working as it should.

- [`auto.c`](test/auto.c) — A set of automated unit tests for the public interface.
- [`triangle.c`](test/triangle.c) — The "Hello Triangle" example in this README.
- [`testcard.c`](test/testcard.c) — A graphical card, similar to [TV test cards](https://en.wikipedia.org/wiki/Test_card), with the goal of ensuring visuals and inputs are working properly.
- [`audio.c`](test/audio.c) — Tests audio functions with various file formats interpreted as sound samples and music.
- [`controller.c`](test/controller.c) — Provides visual and numeric feedback of game controller input.
- [`triangle-ios-tvos.c`](test/triangle-ios-tvos.c) — A modified `triangle.c` designed for iOS and tvOS devices.

### Building and running tests

Run `make test`, or `nmake /f NMakefile test` on Windows using Visual C++, to compile tests to the `test/` directory. The resulting executables will have the same name as their C source file. Since media paths are set relatively in these test programs, make sure to `cd` into the `test/` directory before running a test, for example:

```bash
# on Unix-like systems
make test && cd test/ && ./testcard

# on Windows using MinGW
make test & cd test\ & testcard.exe

# on Windows using Visual C++
nmake /f NMakefile test & cd test\ & testcard.exe
```

Each test also has a makefile target, so you can build and run tests using, for example, `make test testcard`. Or, conveniently uninstall everything, rebuild Simple 2D and tests from source, and run tests using `make rebuild <name_of_test>`, for example:

```bash
# rebuild and run `auto.c` then `testcard.c`

# on Unix-like systems and Windows using MinGW
make rebuild auto testcard

# on Windows using Visual C++
nmake /f NMakefile rebuild auto testcard
```

#### iOS and tvOS

To run the iOS and tvOS test, first run `make frameworks && make install-frameworks` to build and install the iOS and tvOS frameworks. Next, run `make ios` to run the test in an iOS simulator and `make tvos` to run in a tvOS Simulator.

---

# Creating apps with Simple 2D

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

Save the code above to a file called `triangle.c` and compile it by running `simple2d build triangle.c` on the command line (in MinGW, run this in a Bash prompt). Now run the app using `./triangle` on macOS and Linux, or `triangle.exe` on Windows, and enjoy your stunning triangle in a 640x480 window at 60 frames per second!

The `simple2d build` command is a helpful shortcut for compiling a single source file. Of course, you can also use a compiler directly, for example on Unix-like systems:

```bash
cc triangle.c `simple2d --libs` -o triangle
```

And on Windows using Visual C++ in a developer command prompt:

```bash
cl triangle.c /I %LOCALAPPDATA%\simple2d /link /LIBPATH %LOCALAPPDATA%\simple2d\simple2d.lib /SUBSYSTEM:CONSOLE

# as a PowerShell command
iex "cl triangle.c $(simple2d --libs)"
```

## 2D basics

Let's learn more about structuring applications for 2D drawing and more.

### The window

All rendered content, input, and sound is controlled by the window, and so creating a window is the first thing you'll do. Start by declaring a pointer to a `Window` structure and initializing it using `S2D_CreateWindow`.

```c
S2D_Window *window = S2D_CreateWindow(
  "Hello World!",  // title of the window
  800, 600,        // width and height
  update, render,  // callback function pointers (these can be NULL)
  0                // flags
);
```

To set the window width and height to the display's dimensions, use `S2D_DISPLAY_WIDTH` and `S2D_DISPLAY_HEIGHT` for those values, respectively.

The window flags can be `0` or any one of the following:

```c
S2D_RESIZABLE   // allow window to be resized
S2D_BORDERLESS  // show window without a border
S2D_FULLSCREEN  // show window at fullscreen
S2D_HIGHDPI     // enable high DPI mode
```

Flags can also be combined using the bitwise OR operator, for example: `S2D_RESIZABLE | S2D_BORDERLESS`

The viewport can also be set independently of the window size, for example:

```c
window->viewport.width  = 400;
window->viewport.height = 300;
```

The viewport has various scaling modes, such as `S2D_FIXED` (viewport stays the same size as window size changes), `S2D_EXPAND` (viewport expands to fill the window when resized), `S2D_SCALE` (the default, where the viewport scales proportionately and is centered in the window), or `S2D_STRETCH` (viewport stretches to fill the entire window). Set the mode like so:

```c
window->viewport.mode = S2D_FIXED;
```

Before showing the window, this attribute can be set:

```c
window->vsync = false;  // set the vertical sync, true by default
```

Once your window is ready to go, show it using:

```c
S2D_Show(window);
```

Any time before or during the window is being shown, these attributes can be set:

```c
// Cap the frame rate, 60 frames per second by default
window->fps_cap = 30;

// Set the window background color, black by default
window->background.r = 1.0;
window->background.g = 0.5;
window->background.b = 0.8;
window->background.a = 1.0;
```

Callback functions can also be changed any time — more on that below. Many values can also be read from the `Window` structure, see the [`simple2d.h`](include/simple2d.h) header file for details.

When you're done with the window, free it using:

```c
S2D_FreeWindow(window);
```

### Update and render

The window loop is where all the action takes place: the frame rate is set, input is handled, the app state is updated, and visuals are rendered. You'll want to declare two essential functions which will be called by the window loop: `update` and `render`. Like a traditional game loop, `update` is used for updating the application state, and `render` is used for drawing the scene. Simple 2D optimizes both functions for performance and accuracy, so it's good practice to keep those updating and rendering tasks separate.

The update and render functions should look like this:

```c
void update() { /* update your application state */ }
void render() { /* draw stuff */ }
```

Remember to add these function names when calling `S2D_CreateWindow` (see ["The Window"](#the-window) section above for an example).

To exit the window loop at any time, call the following function:

```c
S2D_Close(window);
```

## Drawing basics

Where a vertex is present, like with shapes, there will be six values which need to be set for each: the `x` and `y` coordinates, and four color values. Most values are floats, although `x` and `y` coordinates are typically integers expressed as whole numbers (from 0 to whatever). When vertices have different color values, the space between them are blended in a gradient.

The shorthand for the examples below are:

```c
x = x coordinate
y = y coordinate

// Color range is from 0.0 to 1.0
r = red
g = green
b = blue
a = alpha
```

Using this notation, `x2` would be the second `x` coordinate, and `b2` would be the blue value at that vertex.

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

Lines are drawn with `S2D_DrawLine`:

```c
S2D_DrawLine(x1, y1, x2, y2,
             width,
             r1, g1, b1, a1,
             r2, g2, b2, a2,
             r3, g3, b3, a3,
             r4, g4, b4, a4);
```

### Images

Images in many popular formats, like JPEG, PNG, and BMP can be drawn in the window. Unlike shapes, images need to be read from files and stored in memory. Simply declare a pointer to an `S2D_Image` structure and initialize it using `S2D_CreateImage` providing the file path to the image.

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
img->width  = 256;
img->height = 512;
```

You can also adjust the color of the image like this:

```c
// Default is 1.0 for each, a white color filter
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

Sprites are special kinds of images which can be used to create animations. To create a sprite, declare a pointer to an `S2D_Sprite` structure and initialize it using `S2D_CreateSprite` providing the file path to the sprite sheet image.

```c
S2D_Sprite spr = S2D_CreateSprite("sprite_sheet.png");
```

If the sprite image can't be found, `S2D_CreateSprite` will return `NULL`.

Clip the sprite sheet to a single image using `S2D_ClipSprite` and providing a clipping rectangle:

```c
S2D_ClipSprite(spr, x, y, width, height);
```

The `x, y` position of the sprite itself can be changed like so:

```c
spr->x = 150;
spr->y = 275;
```

Change the size of the sprite by adjusting its width and height:

```c
spr->width  = 100;
spr->height = 100;
```

You can also adjust the color of the sprite image like this:

```c
// Default is 1.0 for each, a white color filter
spr->color.r = 1.0;
spr->color.g = 0.8;
spr->color.b = 0.2;
spr->color.a = 1.0;
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

Text is drawn much like images. Start by finding your favorite OpenType font (with a `.ttf` or `.otf` file extension), then declare a pointer to a `S2D_Text` structure, and initialize it using `S2D_CreateText` providing the file path to the font, the message to display, and the size:

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
// Default is 1.0 for each, a white color filter
txt->color.r = 0.5;
txt->color.g = 1.0;
txt->color.b = 0.0;
txt->color.a = 0.7;
```

Finally, draw the text using:

```c
S2D_DrawText(txt);
```

You can also change the text message at any time. Use `S2D_SetText` and provide the `Text` pointer along with the new message:

```c
S2D_SetText(txt, "A different message!");
```

Since the text was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeText(txt);
```

## Audio

Simple 2D supports a number of popular audio formats, including WAV, MP3, Ogg Vorbis, and FLAC. There are two kinds of audio concepts: sounds and music. Sounds are intended to be short samples, played without interruption, like an effect. Music is for longer pieces which can be played, paused, stopped, resumed, and faded out, like a background soundtrack.

### Sounds

Create a sound by first declaring a pointer to a `S2D_Sound` structure and initialize it using `S2D_CreateSound` providing the path to the audio file:

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

Play the music using `S2D_PlayMusic` providing the pointer and a boolean denoting whether the music should be repeated.

```c
S2D_PlayMusic(mus, true);  // play music looped
```

Only one piece of music can be played at a time. The following functions for pausing, resuming, stopping, and fading out apply to whatever music is currently playing:

```c
S2D_PauseMusic();
S2D_ResumeMusic();
S2D_StopMusic();

// Fade out over 2000 milliseconds, or 2 seconds
S2D_FadeOutMusic(2000);
```

Since music is allocated dynamically, free it using:

```c
S2D_FreeMusic(mus);
```

## Input

Simple 2D can capture input from just about anything. Let's learn how to grab input events from the mouse, keyboard, and game controllers.

### Keyboard

There are three types of keyboard events captured by the window: when a key is pressed down, a key is being held down, and a key is released. When a keyboard event takes place, the window calls its `on_key` function.

To capture keyboard input, first define the `on_key` function and read the event details from the `S2D_Event` structure, for example:

```c
void on_key(S2D_Event e) {
  // Check `e.key` for the key being interacted with

  switch (e.type) {
    case S2D_KEY_DOWN:
      // Key was pressed
      break;

    case S2D_KEY_HELD:
      // Key is being held down
      break;

   case S2D_KEY_UP:
      // Key was released
      break;
  }
}
```

Then, attach the callback to the window:

```c
window->on_key = on_key;
```

### Mouse

The cursor position of the mouse or trackpad can be read at any time from the window. Note that the top, left corner is the origin, `(0, 0)`.

```c
window->mouse.x;
window->mouse.y;
```

To capture mouse button input, first define the `on_mouse` function and read the event details from the `S2D_Event` structure, for example:

```c
// `e.button` can be one of:
//   S2D_MOUSE_LEFT
//   S2D_MOUSE_MIDDLE
//   S2D_MOUSE_RIGHT
//   S2D_MOUSE_X1
//   S2D_MOUSE_X2

void on_mouse(S2D_Event e) {
  switch (e.type) {
    case S2D_MOUSE_DOWN:
      // Mouse button was pressed
      // Use `e.button` to see what button was clicked
      // Check `e.dblclick` to see if was a double click
      break;

    case S2D_MOUSE_UP:
      // Mouse button was released
      // Use `e.button` to see what button was clicked
      // Check `e.dblclick` to see if was a double click
      break;

    case S2D_MOUSE_SCROLL:
      // Mouse was scrolled
      // Check `e.direction` for direction being scrolled, normal or inverted:
      //   S2D_MOUSE_SCROLL_NORMAL
      //   S2D_MOUSE_SCROLL_INVERTED
      // Check `e.delta_x` and `e.delta_y` for the difference in x and y position
      break;

    case S2D_MOUSE_MOVE:
      // Mouse was moved
      // Check `e.delta_x` and `e.delta_y` for the difference in x and y position
      break;
  }
}
```

Then, attach the callback to the window:

```c
window->on_mouse = on_mouse;
```

To show or hide the cursor over the window, use `S2D_ShowCursor`:

```c
// Show the cursor (true by default)...
S2D_ShowCursor(true);

// ...or hide it
S2D_ShowCursor(false);
```

### Game controllers

All game controllers are automatically detected, added, and removed. There are two types of events captured by the window: axis motion and button presses. When a button is pressed or a joystick moved, the window calls its `on_controller` function. Buttons and axes are mapped to a generic Xbox controller layout.

To capture controller input, first define the `on_controller` function and read the event details from the `S2D_Event` structure, for example:

```c
void on_controller(S2D_Event e) {
  // Check `e.which` for the controller being interacted with

  switch (e.type) {
    case S2D_AXIS:
      // Controller axis was moved
      // Use `e.axis` to get the axis, either:
      //   S2D_AXIS_LEFTX, S2D_AXIS_LEFTY,
      //   S2D_AXIS_RIGHTX, S2D_AXIS_RIGHTY,
      //   S2D_AXIS_TRIGGERLEFT, S2D_AXIS_TRIGGERRIGHT,
      //   or S2D_AXIS_INVALID
      // Use `e.value` to get the value of the axis
      break;

    // For the following button events, use `e.button`
    // to get the button pressed or released, which can be:
    //   S2D_BUTTON_A, S2D_BUTTON_B, S2D_BUTTON_X, S2D_BUTTON_Y,
    //   S2D_BUTTON_BACK, S2D_BUTTON_GUIDE, S2D_BUTTON_START,
    //   S2D_BUTTON_LEFTSTICK, S2D_BUTTON_RIGHTSTICK,
    //   S2D_BUTTON_LEFTSHOULDER, S2D_BUTTON_RIGHTSHOULDER,
    //   S2D_BUTTON_DPAD_UP, S2D_BUTTON_DPAD_DOWN,
    //   S2D_BUTTON_DPAD_LEFT, S2D_BUTTON_DPAD_RIGHT,
    //   or S2D_BUTTON_INVALID

    case S2D_BUTTON_DOWN:
      // Controller button was pressed
      break;

    case S2D_BUTTON_UP:
      // Controller button was released
      break;
  }
}
```

Then, attach the callback to the window:

```c
window->on_controller = on_controller;
```

See the [`controller.c`](test/controller.c) test for an exhaustive example of how to interact with game controllers.

You're certain to find controllers that don't yet have button mappings, especially if they're brand new. See the [community-sourced database](https://github.com/gabomdq/SDL_GameControllerDB) of controller mappings for examples of how to generate mappings strings. Once you have the mapping string, you can add it using the `S2D_AddControllerMapping` function, or add a file containing mapping strings using `S2D_LoadControllerMappingsFromFile` and passing the file path.

# Contributing

> "Simple can be harder than complex: You have to work hard to get your thinking clean to make it simple. But it's worth it in the end because once you get there, you can move mountains." — [Steve Jobs](https://en.wikiquote.org/wiki/Steve_Jobs)

Despite the continuing advancement of graphics hardware and software, getting started with simple graphics programming isn't that easy or accessible. We're working to change that.

Check out the [open issues](https://github.com/simple2d/simple2d/issues) and join the [mailing list](https://groups.google.com/d/forum/simple2d). If you're a hardcore C and OS hacker, you should seriously consider contributing to [SDL](https://www.libsdl.org) so we can continue writing games without worrying about the platforms underneath. Take a look at the talks from [Steam Dev Days](http://steamcommunity.com/devdays), especially [Ryan C. Gordon's](https://twitter.com/icculus) talk on [Game Development with SDL 2.0](https://www.youtube.com/watch?v=MeMPCSqQ-34&list=UUStZs-X5W6V3TFJLnwkzN5w).

## Preparing a release

1. [Run tests](#tests) on all supported platforms
2. Update documentation to reflect the current API
3. Update the version number in [`simple2d.sh`](bin/simple2d.sh) and [`simple2d.cmd`](bin/simple2d.cmd), commit changes
4. Create Windows installers (for Visual C++ and MinGW) and Apple frameworks using the `release` make/nmake target
5. Create a [new release](https://github.com/simple2d/simple2d/releases) in GitHub, with tag in the form `v#.#.#`; attach Windows installers and Apple frameworks to release notes
6. Update the [Homebrew tap](https://github.com/simple2d/homebrew-tap):
    - Update formula with new release archive and frameworks resource URLs
    - Calculate the new `sha256` checksums for the release and frameworks archive, using `shasum -a 256 <file>`
    - Run `brew audit --strict ./simple2d.rb` to detect any issues with the formula
    - Test installation of the formula using `brew install ./simple2d.rb`
    - Commit and push changes to the formula
7. 🎉

# About the project

Simple 2D was created by [Tom Black](http://www.blacktm.com), who thought simple graphics programming was way too difficult and decided to do something about it.

Everything is [MIT Licensed](LICENSE.md), so hack away.

Hope you enjoy this project!
