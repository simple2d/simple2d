# Welcome to Simple 2D!

Simple 2D is a small, open-source graphics engine written in C providing basic 2D drawing abilities and access to input devices and sound. It works across many platforms, creating native windows and interacting with hardware using [SDL](http://www.libsdl.org) while rendering content with [OpenGL](https://www.opengl.org).

If you encounter any issues, ping the [mailing list](https://groups.google.com/d/forum/simple2d). Learn about [contributing](#contributing) below.

## Getting Started

Currently, Simple 2D officially supports OS X, Linux, and [Raspberry Pi](https://www.raspberrypi.org) (Raspbian). To compile and install...

### ...on OS X, use [Homebrew](http://brew.sh):

```bash
brew tap simple2d/tap
brew install simple2d
```

### ...on Linux and Raspberry Pi, run the [`simple2d.sh`](simple2d.sh) Bash script.

Everything will be explained along the way and you'll be prompted before any action is taken. To run this script from the web, copy and paste this in your terminal (make sure to copy the entire string – it's rather long):

```bash
url='https://raw.githubusercontent.com/simple2d/simple2d/master/simple2d.sh'; which curl > /dev/null && cmd='curl -fsSL' || cmd='wget -qO -'; bash <($cmd $url) install
```

Of course, you can always just download or clone this repo and run `make && make install`. This obviously won't check for installed dependancies, which is why the script above is helpful.

Once installed, use the `simple2d` command-line utility to update Simple 2D, check for issues, output the libraries needed to compile applications, and more. Run `simple2d --help` to see all available commands and options.

## Running Tests

### Getting the Test Media

To keep the size of this repository small, media needed for tests are checked into the [`test_media`](https://github.com/simple2d/test_media) repo and referenced as a [Git submodule](http://git-scm.com/book/en/v2/Git-Tools-Submodules). After cloning this repo, init the submodule and get its contents by using:

```bash
git submodule init
git submodule update --remote
```

Alternatively, you can clone the `simple2d` repo and init the `test_media` submodule in one step using:

```bash
git clone --recursive https://github.com/simple2d/simple2d.git
```

To get the latest changes from `test_media`, simply run `git submodule update --remote`.

### Building the Tests

Simply run `make tests`. Tests are compiled and placed in the `tests/` directory with the same name as their C source file.

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

Save the code above in a file called `triangle.c`, and compile it using:

```bash
cc triangle.c `simple2d --libs` -o triangle
```

Run it using `./triangle` and enjoy your stunning triangle in a 640x480 window at 60 frames per second.

## 2D Basics

Let's learn about how to structure an application for 2D drawing and more.

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

Event callback functions can also be changed anytime – more on that below. Many values can also be read from the `Window` structure, see the [simple2d.h](include/simple2d.h) header file for details.

When you're done with the window, close it to free allocated memory and shut down drawing and audio subsystems:

```c
S2D_FreeWindow(window);
```

### Update and Render

The window loop is where all the action takes place: the frame rate is set, input is handled, the app state is updated, and visuals are rendered. You'll want to declare two essential functions which will be called by the window loop: `update` and `render`. Like a traditional game loop, `update` is used for updating the application state, and `render` is used for drawing the scene. Simple 2D optimizes both functions for performance and accurate scene composition, so it's good practice to keep those updating and rendering tasks separate (although drawing can actually be done anywhere, but don't make a habit of it).

The update and render functions should look like this:

```c
void update() { /* update your application state */ }
void render() { /* draw stuff */ }
```

Remember to add these function names when calling `S2D_CreateWindow` (see "The Window" section above for an example).

To exit the window loop at anytime, call the following function:

```c
S2D_Close();
```

## Drawing Basics

Where a vertex is present, like with shapes, there will be six values which need to be set for each: the `x` and `y` coordinates, and four color values. All values are floats, although `x` and `y` coordinates are typically expressed as whole numbers (from 0 to whatever). Color values at different vertices are blended with a gradient at their intersection.

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

Images in many popular formats, like JPEG, PNG, and BMP, can also be drawn in the window. Unlike shapes, images need to be read from files and stored in memory. Simply declare a pointer to an `S2D_Image` structure and initialize it using `S2D_CreateImage`, giving it the file path to the image:

```c
S2D_Image *img = S2D_CreateImage("image.png");
```

If the image can't be found, `S2D_CreateImage` will return `NULL`.

Once you have your image, you can then change the `x, y` position like so:

```c
img->x = 125;
img->y = 350;
```

Finally, draw the image using:

```c
S2D_DrawImage(img);
```

Since the image was allocated dynamically, you'll eventually need to free it using:

```c
S2D_FreeImage(img);
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

The cursor position of the mouse or trackpad can be read at any time from the window. Note the top, left corner is the origin, or `(0, 0)`.

```c
window->mouse.x;
window->mouse.y;
```

To capture mouse button presses, attach a callback to the window:

```c
window->on_mouse = on_mouse;
```

Then define the function to do something:

```c
void on_mouse(int x, int y) {
  // mouse clicked at x, y
}
```

### Keyboard

There are two types of keyboard events captured by the window: a single key press and a key held down. When a key is pressed, the window calls it's `on_key` function exactly once, and if the key is being held down, the `on_key_down` function will be repeatedly called with each cycle of the window loop.

To start capturing keyboard input, first define the `on_key` and `on_key_down` functions:

```c
// Do something with `key` in each of these functions
void on_key(const char *key) { ... }
void on_key_down(const char *key) { ... }
```

These functions can actually be named. In this example, their names just match the window structure's member names for simplicity. Once defined, register the function pointers with the window:

```c
window->on_key = on_key;
window->on_key_down = on_key_down;
```

### Game Controllers

This feature hasn't been implemented yet, but Simple 2D will automatically detect controllers and capture their input.

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

Play the music using `S2D_PlayMusic` providing the pointer and the number of times to be repeated. For example, to play the music once set the second parameter to `0`, and use `-1` to repeat forever.

```c
S2D_PlayMusic(mus, -1);
```

Only one piece of music can be played at a time. These functions for pausing, resuming, stopping, and fading out apply to whatever music is currently playing.

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

Despite the continuing advancement of graphics hardware and software, getting started with simple graphics programming isn't that easy or accessible. We’re working to change that.

Check out the [open issues](https://github.com/simple2d/simple2d/issues) and join the [mailing list](https://groups.google.com/d/forum/simple2d). If you're a hardcore C and OS hacker, you should seriously consider contributing to [SDL](https://www.libsdl.org) so we can continue writing games without worrying about the platforms underneath. Take a look at the talks from [Steam Dev Days](http://www.steamdevdays.com), especially [Ryan C. Gordon's](https://twitter.com/icculus) talk on [Game Development with SDL 2.0](https://www.youtube.com/watch?v=MeMPCSqQ-34&list=UUStZs-X5W6V3TFJLnwkzN5w).

## Preparing a Release

1. [Run tests](#running-tests) on all supported platforms
2. Update the version number in files [`VERSION`](VERSION) and [`simple2d.sh`](simple2d.sh), commit changes
3. Create a [new release](https://github.com/simple2d/simple2d/releases) in GitHub, with tag in the form `v#.#.#`
4. Update the [Homebrew tap](https://github.com/simple2d/homebrew-tap):
  - Update formula with new `url`
  - Run `brew audit --strict ./simple2d.rb` to detect issues
  - Calculate new `sha256` using `brew install ./simple2d.rb` (note the "SHA256 mismatch" error, use the "Actual" value)
  - Test installing using same command above

# About the Project

Simple 2D was created by [Tom Black](https://twitter.com/blacktm), who thought simple graphics programming was way too difficult and decided to do something about it.

Everything is [MIT Licensed](LICENSE.md), so hack away.

Hope you enjoy this project!
