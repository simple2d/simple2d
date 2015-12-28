# Welcome to Simple 2D!

Simple 2D is a small, open-source graphics engine written in C providing basic 2D drawing abilities and access to input devices and sound. It works across many platforms, creating native windows and interacting with hardware using [SDL](http://www.libsdl.org) while rendering content with [OpenGL](https://www.opengl.org).

If you encounter any issues, ping the [mailing list](https://groups.google.com/d/forum/simple2d). Learn about [contributing](#contributing) below.

## Getting Started

Currently, Simple 2D officially supports OS X, Linux, and the [Raspberry Pi](http://www.raspberrypi.org) (Raspbian). To compile and install...

...on OS X, use [Homebrew](http://brew.sh):

```bash
brew tap simple2d/tap
brew install simple2d
```

...on Linux and Raspberry Pi, run the [`simple2d.sh`](simple2d.sh) Bash script. Everything will be explained along the way and you'll be prompted before any action is taken. To run this script from the web, copy and paste this in your terminal (make sure to copy the entire string – it's rather long):

```bash
url='https://raw.githubusercontent.com/simple2d/simple2d/master/simple2d.sh'; which curl > /dev/null && cmd='curl -fsSL' || cmd='wget -qO -'; bash <($cmd $url) install
```

Of course, you can always just download or clone this repo and run `make && make install`. This obviously won't check for installed dependancies, which is why the script above is helpful.

Once installed, use the command-line utility to update Simple 2D, check for issues, output the libraries needed to compile applications, and more. Run `simple2d` to see all the available commands and options.

## Running Tests

### Getting the Test Media

To keep the size of this repository small, media needed for tests are checked into the [`test_media`](https://github.com/simple2d/test_media) repo and referenced as a [Git submodule](http://git-scm.com/book/en/v2/Git-Tools-Submodules). After cloning this repo, init the submodule and get its contents by using:

```bash
git submodule init
git submodule update --remote
```

Alternatively, you can clone the repo and update the submodule in one step:

```bash
git clone --recursive https://github.com/simple2d/simple2d.git
```

Simply run `git submodule update --remote` anytime to get the latest changes from `test_media` (i.e. when there's a new commit available).

### Building the Tests

Simply run `make tests`. Built tests are placed in the `tests/` directory with the same name as their C source file.

### Available Tests

- [`testcard.c`](tests/testcard.c) – A graphical card, similar to [testcards from TV](http://en.wikipedia.org/wiki/Testcard), with the goal of making sure all visual and inputs are working properly.
- [`audio.c`](tests/audio.c) – Tests audio functions with various file formats interpreted as sound samples and music.

Run a test using `make tests && cd tests/ && ./<name_of_test>`, for example:

```bash
make tests && cd tests/ && ./testcard
```

Or, conveniently rebuild Simple 2D from source and run tests using `bash test.sh <name_of_test>`, for example:

```bash
# Run testcard.c
bash test.sh testcard
```

---

# Creating Apps with Simple 2D

Making 2D apps is simple! Let's create a window and draw a triangle...

```c
#include <simple2d.h>

Window *window;

void render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}

int main() {
  
  window = S2D_CreateWindow(
    "Hello Triangle", 640, 480, NULL, render, 0
  );
  
  S2D_Show(window);
  return 0;
}
```

Save the code above in a file called `triangle.c`, and compile it using:

```bash
cc triangle.c `simple2d --libs` -o triangle
```

Run it using `./triangle` and enjoy the stunning triangle in a 640x480 window at 60 frames per second.

## 2D Basics

Let's learn about how to structure an application for 2D drawing and more.

### The Window

All rendered content, input, and sound is controlled by the window, and so creating a window is the first thing you'll do. Start by defining a `Window` structure and initializing it using `S2D_CreateWindow()`.

```c
Window *window;

window = S2D_CreateWindow(
  "Hello World!",  // title of the window
  800, 600,        // width and height
  update, render,  // callback function pointers (these can be NULL)
  0                // flags
);
```

The window flags can be any one of the following, and also combined using a bitwise OR, for example: `HELLO | WORLD`.

```c
S2D_RESIZABLE
S2D_BORDERLESS
S2D_FULLSCREEN
S2D_HIGHDPI
```

Use `0` to not set any flags.

Before showing the window, these attributes can be set:

```c
window->vsync = false  // true by default
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

Event callback functions can also be changed anytime – more on that below. Many values can also be read from the `Window` structure, see the [simple2d.h](simple2d.h) header file for details.

When you're done with the window, close it to free allocated memory and shut down drawing and audio subsystems:

```c
S2D_Close(window);
```

### Update and Render

The window loop is where all the action takes place: the frame rate is set, input is handled, the app state is updated, and visuals are rendered. You'll want to declare two essential functions which will be called by the main window loop: `update()` and `render()`. Like a traditional game loop, `update()` is used for updating the application state, and `render()` is used for drawing the scene. Simple 2D optimizes both functions for performance and accurate scene composition, so it's good practice to keep those updating and rendering tasks separate (although drawing can actually be done anywhere, but don't make a habit of it).

The update and render functions should look like this:

```c
void update() { /* update your application state */ }
void render() { /* draw stuff */ }
```

## Drawing Basics

Where a vertex is present, like with shapes, there will be six values which need to be set for each: the `x` and `y` coordinates, and four color values. All values are floats (technically `GLfloat`, but that shouldn't matter), although `x` and `y` coordinates are usually expressed as whole numbers (from 0 to whatever, that is). Color values at different vertices are blended with a gradient at their intersection.

The shorthand for the examples below are:

```c
x = x coordinate
y = y coordinate

// Color range is from 0 to 1
r = red
g = green
b = blue
a = alpha
```

So, for example, `x2` would be the second `x` coordinate, and `b2` would be the blue value at that vertex.

### Drawing Shapes

There are two fundamental shapes available: triangles and quadrilaterals. Triangles are drawn with the function `S2D_DrawTriangle()`:

```c
S2D_DrawTriangle(x1, y1, r1, g1, b1, a1,
                 x2, y2, r2, g2, b2, a2,
                 x3, y3, r3, g3, b3, a3);
```

Quadrilaterals are drawn with `S2D_DrawQuad()`:

```c
S2D_DrawQuad(x1, y1, r1, g1, b1, a1,
             x2, y2, r2, g2, b2, a2,
             x3, y3, r3, g3, b3, a3,
             x4, y4, r4, g4, b4, a4);
```

### Drawing Images

Images in many popular formats, like JPEG, PNG, and BMP, can also be drawn in the window. Unlike shapes, images need to be read from files and stored in memory. First, define and initialize a new `Image` structure using `S2D_CreateImage()` passing the image file path.

```c
Image img;

img = S2D_CreateImage("image.png");
```

You can then change the `x, y` position of the image like so:

```c
img.x = 125;
img.y = 350;
```

Finally, draw the image using:

```c
S2D_DrawImage(img);
```

Since the image was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeImage(img);
```

### Drawing Text

Text is drawn much like images. First, find your favorite TrueType or OpenType font (with a `.ttf` or `.otf` file extension), then define and initialize a new `Text` structure using `S2D_CreateText()`. This function takes the location of the font file, the message to print, and the size.

```c
Text txt;

txt = S2D_CreateText("vera.ttf", "Hello world!", 20);
```

You can then change the `x, y` position of the text, for example:

```c
txt.x = 127;
txt.y = 740;
```

Draw the text using:

```c
S2D_DrawText(txt);
```

You can also change the text message at any time. Use `S2D_SetText()` and pass a pointer to the text structure along with the new message:

```c
S2D_SetText(&txt, "A different message!");
```

Since the text was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeText(txt);
```

## Input

Simple 2D can capture input from just about anything. Let's learn how to grab input events from the mouse, keyboard, and game controllers.

### Mouse

The cursor position of the mouse or trackpad can be read at any time from the window, where the top, left corner is `0, 0`.

```c
window->mouse.x;
window->mouse.y;
```

To capture mouse button presses, attach a callback to the window.

```c
window->on_mouse = on_mouse;
```

Then define the function to do something.

```c
void on_mouse(int x, int y) {
  // mouse clicked at x, y
}
```

### Keyboard

There are two types of keyboard events that are registered by the window: a single key press `on_key()`, and when a key is held down `on_key_down()`. This distinction is useful for when, say for example, the user is making a game character run by holding down the arrow keys, but still wants to catch other keys to make the character jump, shoot, etc.

These two functions are also both optional, but here's what you'll need to do if you choose to implement both. First, create two functions, like so:

```c
// Do something with `key` in each of these functions
void on_key(const char *key) { ... }
void on_key_down(const char *key) { ... }
```

They can be named anything – their names in this example match the window's member names for simplicity. Once declared, the function pointers have to be registered with the window:

```c
window->on_key = on_key;
window->on_key_down = on_key_down;
```

### Game Controllers

This feature isn't implemented yet, but Simple 2D will eventually be able to automatically detect controllers plugged in.

## Audio

Simple 2D supports a number of audio formats, including WAV, MP3, Ogg Vorbis, and FLAC. There are two kinds of audio concepts: sounds and music. Sounds are intended to be short samples, played without interruption. Music is intended to be longer pieces of audio which can be played, paused, stopped, resumed, and faded out. Technical optimizations are made to each respectively.

### Sounds

To create and play a sound, first define and initialize a new `Sound` structure, and initialize it using `S2D_CreateSound()`:

```c
Sound snd;

snd = S2D_CreateSound("sound.wav");
```

The sound can then be played using:

```c
S2D_PlaySound(snd);
```

Since sounds are allocated dynamically, free them using:

```c
S2D_FreeSound(snd);
```

### Music

Similarly, create and play music by first defining and initializing a new `Music` structure, and initialize it using `S2D_CreateMusic()`:

```c
Music mus;

mus = S2D_CreateMusic("music.ogg");
```

The music can then be played using `S2D_PlayMusic()`, providing the structure and the number of times to be repeated. Set the second parameter to `0` to play the music once, or `-1` to repeat forever, for example:

```c
S2D_PlayMusic(mus, -1);
```

Since only one piece of music is intended to be playing, functions for pausing, resuming, stopping, and fading out apply globally. Use these to control the playing music:

```c
S2D_PauseMusic();
S2D_ResumeMusic();
S2D_StopMusic();

// Set fade out duration in milliseconds
S2D_FadeOutMusic(2000);
```

Since music is allocated dynamically, free it using:

```c
S2D_FreeMusic(mus);
```

# Contributing

> "Simple can be harder than complex: You have to work hard to get your thinking clean to make it simple. But it's worth it in the end because once you get there, you can move mountains." ― [Steve Jobs](http://blogs.wsj.com/digits/2011/08/24/steve-jobss-best-quotes)

Despite the continuing advancement of graphics hardware and software, getting started with simple graphics programming isn't that easy or accessible. Join the community working to change this.

Check out the [open issues](https://github.com/simple2d/simple2d/issues) and join the [mailing list](https://groups.google.com/d/forum/simple2d). If you're a hardcore C and OS hacker, you should seriously consider contributing to [SDL](https://www.libsdl.org) so we can continue writing games without worrying about the platforms underneath. Take a look at the talks from [Steam Dev Days](http://www.steamdevdays.com), especially [Ryan C. Gordon's](https://twitter.com/icculus) talk on [Game Development with SDL 2.0](https://www.youtube.com/watch?v=MeMPCSqQ-34&list=UUStZs-X5W6V3TFJLnwkzN5w).

## Preparing a Release

1. [Run tests](#running-tests) on all supported platforms
2. Update version number in files [`VERSION`](VERSION) and [`simple2d.sh`](simple2d.sh), commit changes
3. Create a [new release](https://github.com/simple2d/simple2d/releases) in GitHub, with tag in the form `v#.#.#`
4. Update the [Homebrew tap](https://github.com/simple2d/homebrew-tap):
  - Update formula with new `url`
  - Run `brew audit --strict ./simple2d.rb` to detect issues
  - Calculate new `sha256` using `brew install ./simple2d.rb` (note the "SHA256 mismatch" error, use the "Actual" value)
  - Test installing using same command above

# About the Project

Simple 2D was created by [Tom Black](https://twitter.com/blacktm) who thought simple graphics programming was way too difficult and decided to do something about it. And no, the web browser is not an acceptable alternative – it's a workaround.

Everything is [MIT Licensed](LICENSE.md), so hack away.

Hope you enjoy this project!
