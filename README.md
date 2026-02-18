# Welcome to Simple 2D!

Simple 2D is a lightweight, open-source 2D graphics engine that offers core drawing, media, and input functionality. It's written in C, runs on multiple platforms, and handles native window creation, rendering, and hardware interaction by leveraging [SDL](https://www.libsdl.org) under the hood.

Simple 2D is ideal for learning the fundamentals of C and SDL without fussing with complicated frameworks. It has a minimal, straightforward API that lets you start drawing graphics, handling input, and playing audio with little setup or boilerplate. So whether you're building a small game, exploring a new idea, or learning how 2D graphics systems work, Simple 2D provides an accessible and welcoming environment for creation and discovery.

> [!IMPORTANT]
> Simple 2D is being updated for [SDL3](https://wiki.libsdl.org/SDL3), and with it comes many architectural changes. If you've used Simple 2D in the past, please read through the documentation carefully and take note of these changes. If you find a bug or something doesn't look right, feel free open an issue. Thanks!

Please note this README will be continuously updated as new features are added, bugs are fixed, and other changes are made. [View the release notes](https://github.com/simple2d/simple2d/releases) for a link to that version's documentation.

## Contents

- [Getting started](#getting-started)
- [Tests](#tests)
- [Creating apps with Simple 2D](#creating-apps-with-simple-2d)
  - [2D basics](#2d-basics)
    - [The window](#the-window)
    - [App lifecycle](#app-lifecycle)
  - [Drawing](#drawing)
    - [Shapes](#shapes)
    - [Images](#images)
    - [Text](#text)
  - [Audio](#audio)
  - [Input](#input)
    - [Keyboard](#keyboard)
    - [Mouse](#mouse)
    - [Game controllers](#game-controllers)
- [About the project](#about-the-project)
- [Troubleshooting](#troubleshooting)

---

# Getting started

Simple 2D supports macOS, Windows, and Linux, both ARM64 and X86 on all platforms.

> [!IMPORTANT]
> At the time of this writing, [SDL3_mixer](https://wiki.libsdl.org/SDL3_mixer) is still in development, so SDL dependencies must be built from source and linked as static libraries. But don't worry, the Makefile will do all the work.

**On macOS**, make sure you have command-line tools available for development. We recommend using [Homebrew](https://brew.sh), which installs Command Line Tools for Xcode for you, and makes it easy to install additional packages needed, like `cmake`.

**On Linux**, SDL recommends [installing these packages](https://wiki.libsdl.org/SDL3/README-linux) based on the distribution.

**On Windows**, we recommend using [MSYS2](https://www.msys2.org).
- For x86_64 systems, use the `MSYS2 UCRT64` terminal, then install the following packages: `pacman -S git make mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-cmake`
- For ARM64 systems, use the `MSYS2 CLANGARM64` terminal, then install the following packages: `pacman -S git make mingw-w64-clang-aarch64-toolchain mingw-w64-clang-aarch64-cmake`

Next, clone this repo:

```sh
git clone --recursive https://github.com/simple2d/simple2d.git
```

Then, `cd` into the `simple2d` directory, and download and build the SDL3 dependencies by running `make deps`.

# Tests

Simple 2D has several test programs to make sure everything is working as it should. Build these tests by running `make test`.

- [`auto.c`](test/auto.c) — A set of automated unit tests for the public interface.
- [`triangle.c`](test/triangle.c) — The "Hello Triangle" example in this README.
- [`testcard.c`](test/testcard.c) — A graphical card, similar to [TV test cards](https://en.wikipedia.org/wiki/Test_card), with the goal of ensuring visuals and inputs are working properly.
- [`sounds.c`](test/sounds.c) — Tests audio functions with various file formats interpreted as both sound samples and music.
- [`controller.c`](test/controller.c) — Provides visual and numeric feedback of game controller input.

Each test also has a makefile target, so you can build and run tests using, for example, `make run_testcard`.

---

# Creating apps with Simple 2D

Making 2D apps is simple! Let's create a window and draw a triangle...

```c
#include <simple2d.h>

void S2D_Init() {
  S2D_CreateWindow("Hello Triangle", 640, 480);
}

void S2D_Render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}
```

Save the code above to a file called `triangle.c`, and make sure `simple2d` is cloned in the same directory. Compile it by running `./simple2d/bin/simple2d.sh build triangle.c` on the command line. Now run the app using `./triangle`.

The `build` command is a helpful shortcut for compiling a single source file. Of course, you can also use a compiler directly:

```bash
cc triangle.c `./simple2d/bin/simple2d.sh --libs` -o triangle
```

## 2D basics

Let's learn about structuring applications for 2D drawing and more.

### The window

All rendered content, input, and sound is controlled by the window, so creating a window is the first thing you'll do. Start by declaring a pointer to a `S2D_Window` structure and initialize it using `S2D_CreateWindow()`:

```c
S2D_Window *window = S2D_CreateWindow(
  "Hello World!",  // title of the window
  800, 600         // width and height
);
```

Any time before or during the window is being shown, these attributes can be set:

```c
// Set the window background color (black by default)
window->background.r = 1.0;
window->background.g = 0.5;
window->background.b = 0.8;
window->background.a = 1.0;
```

Many values can be read from the `S2D_Window` structure, including display width, height, pixel density, and refresh rate. See the [`simple2d.h`](include/simple2d.h) header file for details.

To enable diagnostics mode, which shows the FPS on screen and logs additional info to the console:

```c
S2D_Diagnostics(true);
```

You can also enable diagnostics from the command line by passing `--diagnostics` when running your app.

### App lifecycle

Simple 2D takes care of the application lifecycle for you. Just define any of the callback functions you need, and Simple 2D will call them at the right time. All callbacks are optional, so if you leave one out, it's simply skipped. You also don't need to write your own main loop or poll events manually, Simple 2D handles that for you too.

**`S2D_Init()`** is called once at startup, before the window is shown. This is where you create the window, load images, fonts, audio, and set up your initial state:

```c
void S2D_Init() {
  S2D_CreateWindow("My App", 800, 600);
  // Load resources, initialize state...
}
```

**`S2D_Update()`** and **`S2D_Render()`** are called every frame, like a traditional game loop. Use `S2D_Update()` for updating the application state, and `S2D_Render()` for drawing the scene:

```c
void S2D_Update() { /* update your application state */ }
void S2D_Render() { /* draw stuff */ }
```

**`S2D_Quit()`** is called once when the app is closing. Use it to free any resources you've allocated:

```c
void S2D_Quit() {
  S2D_FreeImage(img);
  S2D_FreeAudio(aud);
}
```

To exit the app at any time, use:

```c
S2D_Close();
```

## Drawing

All kinds of shapes and textures can be drawn in the window. Simple 2D uses modern rendering APIs, so all drawing is DPI-aware and uses hardware acceleration where available. Learn about each drawing feature below.

### Shapes

Several geometric shapes are available, like triangles, quadrilaterals (which rectangles and squares can be made from), lines, and circles. Every shape contains vertices, that is, places where two lines meet to form an angle (a triangle has three, for example). For each vertex of a triangle and quadrilateral, there are six values which need to be set: the `x` and `y` coordinates, and four color values. Lines have two vertices, although colors for each corner can be set. Circles have a single center point and color that can be set. When vertices have different color values, the space between them are blended in a gradient.

The shorthand for the examples below are:

```c
x = the x coordinate
y = the y coordinate

// Color range is from 0.0 to 1.0
r = red
g = green
b = blue
a = alpha (opacity)
```

Using this notation, `x2` would be the second `x` coordinate, and `b2` would be the blue value at that vertex.

To draw a triangle, use:

```c
S2D_DrawTriangle(x1, y1, r1, g1, b1, a1,
                 x2, y2, r2, g2, b2, a2,
                 x3, y3, r3, g3, b3, a3);
```

To draw a quadrilateral, use:

```c
S2D_DrawQuad(x1, y1, r1, g1, b1, a1,
             x2, y2, r2, g2, b2, a2,
             x3, y3, r3, g3, b3, a3,
             x4, y4, r4, g4, b4, a4);
```

To draw a line, use:

```c
S2D_DrawLine(x1, y1, x2, y2,
             width,
             r1, g1, b1, a1,
             r2, g2, b2, a2,
             r3, g3, b3, a3,
             r4, g4, b4, a4);
```

To draw a circle, use:

```c
S2D_DrawCircle(x, y, radius, sectors, r, g, b, a);
```

### Images

Images in many popular formats, like JPEG, PNG, and BMP can be drawn in the window. Unlike shapes, images need to be read from files and stored in memory. Simply declare a pointer to an `S2D_Image` structure and initialize it using `S2D_CreateImage()` providing the file path to the image.

```c
S2D_Image *img = S2D_CreateImage("image.png");
```

If the image can't be found, it will return `NULL`.

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

Rotate the image like so:

```c
// Angle should be in degrees
// The last parameter is the point the image should rotate around, either:
//   S2D_CENTER, S2D_TOP_LEFT, S2D_TOP_RIGHT, S2D_BOTTOM_LEFT, or S2D_BOTTOM_RIGHT
S2D_RotateImage(img, angle, S2D_CENTER);

// Or, set a custom point to rotate around
img->rx = 50;
img->ry = 75;

// Set the rotation angle directly
img->rotate = 90;
```

You can clip an image to show only a portion of it:

```c
S2D_ClipImage(img, x, y, width, height);
```

To remove the clipping and show the full image again:

```c
S2D_UnclipImage(img);
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

Since images are allocated dynamically, free them using:

```c
S2D_FreeImage(img);
```

### Text

Text is drawn much like images. Start by finding your favorite OpenType font (with a `.ttf` or `.otf` file extension), then declare a pointer to a `S2D_Text` structure and initialize it using `S2D_CreateText()` providing the file path to the font, the message to display, and the size.

```c
S2D_Text *txt = S2D_CreateText("vera.ttf", "Hello world!", 20);
```

If the font file can't be found, it will return `NULL`.

You can then change the `x, y` position of the text, for example:

```c
txt->x = 127;
txt->y = 740;
```

Rotate the text like so:

```c
// Angle should be in degrees
// The last parameter is the point the text should rotate around, either:
//   S2D_CENTER, S2D_TOP_LEFT, S2D_TOP_RIGHT, S2D_BOTTOM_LEFT, or S2D_BOTTOM_RIGHT
S2D_RotateText(txt, angle, S2D_CENTER);

// Or, set a custom point to rotate around
txt->rx = 50;
txt->ry = 75;

// Set the rotation angle directly
txt->rotate = 90;
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

You can also change the text message at any time:

```c
S2D_SetText(txt, "A different message!");

// Format text just like `printf`
S2D_SetText(txt, "Welcome %s!", player);
```

Since text is allocated dynamically, free them using:

```c
S2D_FreeText(txt);
```

## Audio

Simple 2D supports a number of popular audio formats, including WAV, MP3, Ogg Vorbis, and FLAC. All audio can be played, paused, stopped, resumed, and faded out.

Create audio by first declaring a pointer to a `S2D_Audio` structure and initialize it using `S2D_CreateAudio()` providing the path to the audio file.

```c
S2D_Audio *aud = S2D_CreateAudio("sound.wav");
```

If the audio file can't be found, it will return `NULL`.

Play the sound like this:

```c
S2D_PlayAudio(aud);
```

You can pause, resume, and stop the audio (optionally with fade-out):

```c
S2D_PauseAudio(aud);
S2D_ResumeAudio(aud);
S2D_StopAudio(aud, 0);      // stop immediately
S2D_StopAudio(aud, 500);    // fade out over 500 ms
```

You can get and set the volume of a sound as a percentage (0–100):

```c
int volume = S2D_GetAudioVolume(aud);
S2D_SetAudioVolume(aud, 50);  // set volume to 50%
```

You can also get and set the volume of all sounds (the mixer) as a percentage:

```c
int volume = S2D_GetAudioMixerVolume();
S2D_SetAudioMixerVolume(50);  // set mixer volume to 50%
```

Since sounds are allocated dynamically, free them using:

```c
S2D_FreeAudio(aud);
```

## Input

Simple 2D can capture input from just about anything. Let's learn how to grab input events from the mouse, keyboard, and game controllers.

### Keyboard

There are three types of keyboard events captured by the window: when a key is pressed down, a key is being held down, and a key is released. When a keyboard event takes place, the window calls its `S2D_OnKey()` function.

To capture keyboard input, first define the `S2D_OnKey()` function and read the event details from the `S2D_Event` structure, for example:

```c
void S2D_OnKey(S2D_Event e) {
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

You can use `S2D_KeyIs()` to check if a specific key was involved in the event:

```c
if (S2D_KeyIs(e, "Escape")) S2D_Close();
if (S2D_KeyIs(e, "Space")) jump();
```

### Mouse

The cursor position of the mouse or trackpad can be read at any time from the window. Note that the top, left corner is the origin, `(0, 0)`.

```c
window->mouse.x;
window->mouse.y;
```

To capture mouse button input, first define the `S2D_OnMouse()` function and read the event details from the `S2D_Event` structure, for example:

```c
// `e.button` can be one of:
//   S2D_MOUSE_LEFT
//   S2D_MOUSE_MIDDLE
//   S2D_MOUSE_RIGHT
//   S2D_MOUSE_X1
//   S2D_MOUSE_X2

void S2D_OnMouse(S2D_Event e) {
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

### Game controllers

All game controllers are automatically detected, added, and removed. There are two types of events captured by the window: axis motion and button presses. When a button is pressed or a joystick moved, the window calls its `S2D_OnController()` function. Buttons and axes are mapped to a generic Xbox controller layout.

To capture controller input, first define the `S2D_OnController()` function and read the event details from the `S2D_Event` structure, for example:

```c
void S2D_OnController(S2D_Event e) {
  // Check `e.id` for the controller being interacted with

  switch (e.type) {
    case S2D_AXIS:
      // Controller axis was moved
      // Use `e.axis` to get the axis
      // Use `e.value` to get the value of the axis
      break;

    case S2D_BUTTON_DOWN:
      // Controller button was pressed
      // Use `e.button` to get the button pressed
      break;

    case S2D_BUTTON_UP:
      // Controller button was released
      // Use `e.button` to get the button released
      break;

    case S2D_CONTROLLER_ADDED:
      // Controller was added
      break;

    case S2D_CONTROLLER_REMOVED:
      // Controller was removed
      break;
  }
}
```

See the [`controller.c`](test/controller.c) test for an exhaustive example of how to interact with game controllers.

# About the project

> "Simple can be harder than complex: you have to work hard to get your thinking clean to make it simple. But it's worth it in the end because once you get there, you can move mountains." — Steve Jobs

Despite advancements, getting started with simple graphics programming isn't that easy, but we can change that. Simple 2D was created by [Tom Black](http://www.blacktm.com), who thought drawing a triangle to the screen was unnecessarily difficult.

Everything is [MIT Licensed](LICENSE.md), so hack away!

# Troubleshooting

If building and testing in a virtual machine, some SDL subsystems might not be available.

On Linux / Ubuntu, if you get `Error: (SDL_Init) No available audio device`, run a command with `SDL_AUDIO_DRIVER=dummy` prefixed, for example:

```sh
SDL_AUDIO_DRIVER=dummy make run_testcard
```
