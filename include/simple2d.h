// simple2d.h

#ifndef SIMPLE2D_H
#define SIMPLE2D_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MINGW32__
// Define to get GNU extension functions and types, like `vasprintf()` and M_PI
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#endif

#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

// This flag is set in the Simple 2D Makefile when building the library (`-DS2D_NO_SDL_MAIN`)
#ifndef S2D_NO_SDL_MAIN
  #define SDL_MAIN_USE_CALLBACKS
  #include <SDL3/SDL_main.h>
#endif

#ifdef _WIN32
  #include <io.h>
  #define  F_OK 0  // for testing file existence
  #define access _access
#else
  #include <unistd.h>
#endif

// Simple 2D Definitions ///////////////////////////////////////////////////////

// Messages
#define S2D_INFO  1
#define S2D_WARN  2
#define S2D_ERROR 3

// Positions
#define S2D_CENTER       1
#define S2D_TOP_LEFT     2
#define S2D_TOP_RIGHT    3
#define S2D_BOTTOM_LEFT  4
#define S2D_BOTTOM_RIGHT 5

// Keyboard events
#define S2D_KEY_DOWN 1  // key is pressed
#define S2D_KEY_HELD 2  // key is held down
#define S2D_KEY_UP   3  // key is released

// Mouse events
#define S2D_MOUSE_DOWN   1  // mouse button pressed
#define S2D_MOUSE_UP     2  // mouse button released
#define S2D_MOUSE_SCROLL 3  // mouse scrolling or wheel movement
#define S2D_MOUSE_MOVE   4  // mouse movement

// Mouse buttons
#define S2D_MOUSE_LEFT   SDL_BUTTON_LEFT
#define S2D_MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define S2D_MOUSE_RIGHT  SDL_BUTTON_RIGHT
#define S2D_MOUSE_X1     SDL_BUTTON_X1
#define S2D_MOUSE_X2     SDL_BUTTON_X2
#define S2D_MOUSE_SCROLL_NORMAL   SDL_MOUSEWHEEL_NORMAL
#define S2D_MOUSE_SCROLL_INVERTED SDL_MOUSEWHEEL_FLIPPED

// Controller events
#define S2D_CONTROLLER_ADDED   1
#define S2D_CONTROLLER_REMOVED 2
#define S2D_AXIS               3
#define S2D_BUTTON_DOWN        4
#define S2D_BUTTON_UP          5

// Controller axis labels
#define S2D_AXIS_INVALID       SDL_GAMEPAD_AXIS_INVALID
#define S2D_AXIS_LEFTX         SDL_GAMEPAD_AXIS_LEFTX
#define S2D_AXIS_LEFTY         SDL_GAMEPAD_AXIS_LEFTY
#define S2D_AXIS_RIGHTX        SDL_GAMEPAD_AXIS_RIGHTX
#define S2D_AXIS_RIGHTY        SDL_GAMEPAD_AXIS_RIGHTY
#define S2D_AXIS_LEFT_TRIGGER  SDL_GAMEPAD_AXIS_LEFT_TRIGGER
#define S2D_AXIS_RIGHT_TRIGGER SDL_GAMEPAD_AXIS_RIGHT_TRIGGER
#define S2D_AXIS_COUNT         SDL_GAMEPAD_AXIS_COUNT

// Controller button labels
#define S2D_BUTTON_INVALID        SDL_GAMEPAD_BUTTON_INVALID
#define S2D_BUTTON_SOUTH          SDL_GAMEPAD_BUTTON_SOUTH
#define S2D_BUTTON_EAST           SDL_GAMEPAD_BUTTON_EAST
#define S2D_BUTTON_WEST           SDL_GAMEPAD_BUTTON_WEST
#define S2D_BUTTON_NORTH          SDL_GAMEPAD_BUTTON_NORTH
#define S2D_BUTTON_BACK           SDL_GAMEPAD_BUTTON_BACK
#define S2D_BUTTON_GUIDE          SDL_GAMEPAD_BUTTON_GUIDE
#define S2D_BUTTON_START          SDL_GAMEPAD_BUTTON_START
#define S2D_BUTTON_LEFT_STICK     SDL_GAMEPAD_BUTTON_LEFT_STICK
#define S2D_BUTTON_RIGHT_STICK    SDL_GAMEPAD_BUTTON_RIGHT_STICK
#define S2D_BUTTON_LEFT_SHOULDER  SDL_GAMEPAD_BUTTON_LEFT_SHOULDER
#define S2D_BUTTON_RIGHT_SHOULDER SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER
#define S2D_BUTTON_DPAD_UP        SDL_GAMEPAD_BUTTON_DPAD_UP
#define S2D_BUTTON_DPAD_DOWN      SDL_GAMEPAD_BUTTON_DPAD_DOWN
#define S2D_BUTTON_DPAD_LEFT      SDL_GAMEPAD_BUTTON_DPAD_LEFT
#define S2D_BUTTON_DPAD_RIGHT     SDL_GAMEPAD_BUTTON_DPAD_RIGHT
#define S2D_BUTTON_COUNT          SDL_GAMEPAD_BUTTON_COUNT

// Type Definitions ////////////////////////////////////////////////////////////

// S2D_Event
typedef struct {
  int type;
  int id;
  const char *name;
  int button;
  bool dblclick;
  const char *key;
  int x;
  int y;
  int delta_x;
  int delta_y;
  int direction;
  int axis;
  int value;
} S2D_Event;

// S2D_Point, for graphics calculations
typedef struct {
  float x;
  float y;
} S2D_Point;

// S2D_Color
typedef struct {
  float r;
  float g;
  float b;
  float a;
} S2D_Color;

// S2D_Mouse
typedef struct {
  int visible;
  int x;
  int y;
} S2D_Mouse;

// S2D_Viewport
typedef struct {
  int width;
  int height;
  int mode;
} S2D_Viewport;

// S2D_Window
typedef struct {
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;
  const char *title;
  int width;
  int height;
  int orig_width;
  int orig_height;
  int display_width;
  int display_height;
  float display_scale;
  float display_content_scale;
  float display_refresh_rate;
  S2D_Viewport viewport;
  int flags;
  S2D_Mouse mouse;
  bool vsync;
  S2D_Color background;
  const char *icon;
  uint64_t frames;
  double fps;
  bool close;
} S2D_Window;

// S2D_Image
typedef struct {
  const char *path;
  SDL_Surface *surface;
  SDL_Texture *texture;
  S2D_Color color;
  int x;
  int y;
  int width;
  int height;
  int orig_width;
  int orig_height;
  bool clipped;
  int clip_x;
  int clip_y;
  int clip_width;
  int clip_height;
  float rotate;  // Rotation angle in degrees
  float rx;      // X coordinate to be rotated around
  float ry;      // Y coordinate to be rotated around
} S2D_Image;

// S2D_Text
typedef struct {
  char *msg;
  SDL_Surface *surface;
  SDL_Texture *texture;
  const char *font;
  TTF_Font *font_data;
  S2D_Color color;
  int x;
  int y;
  int width;
  int height;
  float rotate;  // Rotation angle in degrees
  float rx;      // X coordinate to be rotated around
  float ry;      // Y coordinate to be rotated around
} S2D_Text;

// S2D_Audio
typedef struct {
  char *path;
  MIX_Audio *mix_audio;
  MIX_Track *mix_track;
} S2D_Audio;

// S2D_App
typedef struct {
  S2D_Window *window;
  MIX_Mixer *sdl_mixer;
  bool diagnostics;
  bool quit;
} S2D_App;

// Make the Simple 2D app globally accessible
extern S2D_App s2d_app;

// Simple 2D Functions /////////////////////////////////////////////////////////

// User-defined Simple 2D functions
extern void S2D_Init();
extern void S2D_OnKey(S2D_Event e);
extern void S2D_OnMouse(S2D_Event e);
extern void S2D_OnController(S2D_Event e);
extern void S2D_Update();
extern void S2D_Render();
extern void S2D_Quit();

/*
 * Checks if a file exists and can be accessed
 */
bool S2D_FileExists(const char *path);

/*
 * Logs standard messages to the console
 */
void S2D_Log(int type, const char *msg, ...);

/*
 * Logs Simple 2D errors to the console, with caller and message body
 */
void S2D_Error(const char *caller, const char *msg, ...);

/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status);

/*
 * Enable terminal colors in Windows
 */
void S2D_Windows_EnableTerminalColors();

/*
 * Returns true if the given S2D_Event's key matches the specified key string
 */
bool S2D_KeyIs(S2D_Event e, const char *key);

// Shapes //////////////////////////////////////////////////////////////////////

/*
 * Rotate a point around a given point
 * Params:
 *   p      The point to rotate
 *   angle  The angle in degrees
 *   rx     The x coordinate to rotate around
 *   ry     The y coordinate to rotate around
 */
S2D_Point S2D_RotatePoint(S2D_Point p, float angle, float rx, float ry);

/*
 * Get the point to be rotated around given a position in a rectangle
 */
S2D_Point S2D_GetRectRotationPoint(int x, int y, int w, int h, int position);

/*
 * Draw a triangle
 */
void S2D_DrawTriangle(
  float x1, float y1,
  float r1, float g1, float b1, float a1,
  float x2, float y2,
  float r2, float g2, float b2, float a2,
  float x3, float y3,
  float r3, float g3, float b3, float a3
);

/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(
  float x1, float y1,
  float r1, float g1, float b1, float a1,
  float x2, float y2,
  float r2, float g2, float b2, float a2,
  float x3, float y3,
  float r3, float g3, float b3, float a3,
  float x4, float y4,
  float r4, float g4, float b4, float a4
);

/*
 * Draw a line from a quad
 */
void S2D_DrawLine(
  float x1, float y1, float x2, float y2,
  float width,
  float r1, float g1, float b1, float a1,
  float r2, float g2, float b2, float a2,
  float r3, float g3, float b3, float a3,
  float r4, float g4, float b4, float a4
);

/*
 * Draw a circle from triangles
 */
void S2D_DrawCircle(
  float x, float y, float radius, int sectors,
  float r, float g, float b, float a
);

// Image ///////////////////////////////////////////////////////////////////////

/*
 * Create an image, given a file path
 */
S2D_Image *S2D_CreateImage(const char *path);

/*
 * Rotate an image
 */
void S2D_RotateImage(S2D_Image *img, float angle, int position);

/*
 * Set the clipping rectangle for an image
 */
void S2D_ClipImage(S2D_Image *img, int x, int y, int width, int height);

/*
 * Remove the clipping rectangle from an image
 */
void S2D_UnclipImage(S2D_Image *img);

/*
 * Draw an image
 */
void S2D_DrawImage(S2D_Image *img);

/*
 * Free an image
 */
void S2D_FreeImage(S2D_Image *img);

// Text ////////////////////////////////////////////////////////////////////////

/*
 * Create text, given a font file path, the message, and size
 */
S2D_Text *S2D_CreateText(const char *font, const char *msg, int size);

/*
* Set the text message
*/
void S2D_SetText(S2D_Text *txt, const char *msg, ...);

/*
 * Rotate text
 */
void S2D_RotateText(S2D_Text *txt, float angle, int position);

/*
 * Draw text
 */
void S2D_DrawText(S2D_Text *txt);

/*
 * Free the text
 */
void S2D_FreeText(S2D_Text *txt);

// Audio ///////////////////////////////////////////////////////////////////////

/*
 * Create audio, given an audio file path
 */
S2D_Audio *S2D_CreateAudio(const char *path);

/*
 * Play the audio
 */
void S2D_PlayAudio(S2D_Audio *audio);

/*
 * Pause the audio
 */
void S2D_PauseAudio(S2D_Audio *audio);

/*
 * Resume the audio
 */
void S2D_ResumeAudio(S2D_Audio *audio);

/*
 * Stop the audio
 */
void S2D_StopAudio(S2D_Audio *audio, int ms_fade);

/*
 * Get the audio's volume (percentage, 0-100)
 */
int S2D_GetAudioVolume(S2D_Audio *audio);

/*
 * Set the audio volume by percentage (0-100)
 */
void S2D_SetAudioVolume(S2D_Audio *audio, int volume);

/*
 * Get the audio mixer volume (percentage, 0-100)
 */
int S2D_GetAudioMixerVolume();

/*
 * Set the audio mixer volume a given percentage (0-100)
 */
void S2D_SetAudioMixerVolume(int volume);

/*
 * Free the audio
 */
void S2D_FreeAudio(S2D_Audio *audio);

// Window //////////////////////////////////////////////////////////////////////

/*
 * Create a window
 */
S2D_Window *S2D_CreateWindow(
  const char *title, int width, int height
);

/*
 * Show the window
 */
bool S2D_ShowWindow();

/*
 * Map window to renderer coordinates
 */
void S2D_WindowToRendererCoordinates(SDL_Vertex *vertices, int count);

/*
 * Map window to renderer coordinates for a rectangle
 */
void S2D_WindowToRendererCoordinatesRect(SDL_FRect *rect);

/*
 * Close the app
 */
void S2D_Close();

/*
 * Free all resources
 */
void S2D_FreeWindow(S2D_Window *window);


#ifdef __cplusplus
}
#endif

#endif // SIMPLE2D_H
