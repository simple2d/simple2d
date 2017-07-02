// simple2d.h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
// Set Platform Constants //////////////////////////////////////////////////////

// If ARM, assume GLES
#ifdef __arm__
  #define GLES true
#else
  #define GLES false
#endif

// If Windows
#ifdef _WIN32
  #define WINDOWS true
#endif

// If Windows and MinGW
#ifdef __MINGW32__
  #define MINGW true
#endif

// Includes ////////////////////////////////////////////////////////////////////

#if WINDOWS && !MINGW
  #include <io.h>
  #define  F_OK 0  // For testing file existence
#else
  #include <unistd.h>
#endif

#if WINDOWS
  #include <windows.h>
  // For terminal colors
  #ifndef  ENABLE_VIRTUAL_TERMINAL_PROCESSING
  #define  ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
  #endif
#endif

// SDL
#include <SDL2/SDL.h>

// If MinGW, undefine `main()` from SDL_main.c
#if MINGW
  #undef main
#endif

// OpenGL
#if GLES
  #include <SDL2/SDL_opengles2.h>
#else
  #define GL_GLEXT_PROTOTYPES 1
  #if WINDOWS
    #include <glew.h>
  #endif
  #include <SDL2/SDL_opengl.h>
#endif

// SDL libraries
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Simple 2D Definitions ///////////////////////////////////////////////////////

// Messages
#define S2D_INFO  1
#define S2D_WARN  2
#define S2D_ERROR 3

// Window attributes
#define S2D_RESIZABLE  SDL_WINDOW_RESIZABLE
#define S2D_BORDERLESS SDL_WINDOW_BORDERLESS
#define S2D_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP
#define S2D_HIGHDPI    SDL_WINDOW_ALLOW_HIGHDPI

// Viewport scaling modes
#define S2D_FIXED   1
#define S2D_SCALE   2
#define S2D_STRETCH 3

// Keyboard events
#define S2D_KEY_DOWN 1  // key is pressed
#define S2D_KEY_HELD 2  // key is held down
#define S2D_KEY_UP   3  // key is released

// Mouse events
#define S2D_MOUSE_DOWN   1  // mouse button pressed
#define S2D_MOUSE_UP     2  // mouse button released
#define S2D_MOUSE_SCROLL 3  // mouse scrolling or wheel movement
#define S2D_MOUSE_MOVE   4  // mouse movement
#define S2D_MOUSE_LEFT   SDL_BUTTON_LEFT
#define S2D_MOUSE_MIDDLE SDL_BUTTON_MIDDLE
#define S2D_MOUSE_RIGHT  SDL_BUTTON_RIGHT
#define S2D_MOUSE_X1     SDL_BUTTON_X1
#define S2D_MOUSE_X2     SDL_BUTTON_X2
#define S2D_MOUSE_SCROLL_NORMAL   SDL_MOUSEWHEEL_NORMAL
#define S2D_MOUSE_SCROLL_INVERTED SDL_MOUSEWHEEL_FLIPPED

// Controller events
#define S2D_AXIS        1
#define S2D_BUTTON_DOWN 2
#define S2D_BUTTON_UP   3

// Touch events
#define S2D_FINGER_DOWN   1
#define S2D_FINGER_UP     2
#define S2D_FINGER_MOTION 3

// Internal Shared Data ////////////////////////////////////////////////////////

extern char S2D_msg[1024];    // for S2D_Log messages
extern bool S2D_diagnostics;  // flag for whether to print diagnostics with S2D_Log

// Type Definitions ////////////////////////////////////////////////////////////

// S2D_Event
typedef struct {
  int which;
  int type;
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
  int64_t finger_id;
  int64_t touch_id;
} S2D_Event;

typedef void (*S2D_Update)();
typedef void (*S2D_Render)();
typedef void (*S2D_On_Key)(S2D_Event e);
typedef void (*S2D_On_Mouse)(S2D_Event e);
typedef void (*S2D_On_Controller)(S2D_Event e);
typedef void (*S2D_On_Touch)(S2D_Event e);

// S2D_Color
typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
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
  SDL_Window *sdl;
  SDL_GLContext glcontext;
  const GLubyte *S2D_GL_VENDOR;
  const GLubyte *S2D_GL_RENDERER;
  const GLubyte *S2D_GL_VERSION;
  GLint S2D_GL_MAJOR_VERSION;
  GLint S2D_GL_MINOR_VERSION;
  const GLubyte *S2D_GL_SHADING_LANGUAGE_VERSION;
  const char *title;
  int width;
  int height;
  int orig_width;
  int orig_height;
  S2D_Viewport viewport;
  S2D_Update update;
  S2D_Render render;
  int flags;
  S2D_Mouse mouse;
  S2D_On_Key on_key;
  S2D_On_Mouse on_mouse;
  S2D_On_Controller on_controller;
  S2D_On_Touch on_touch;
  bool vsync;
  int fps_cap;
  S2D_Color background;
  Uint32 frames;
  Uint32 elapsed_ms;
  Uint32 loop_ms;
  Uint32 delay_ms;
  double fps;
  bool close;
} S2D_Window;

// S2D_Image
typedef struct {
  SDL_Surface *surface;
  int format;
  GLuint texture_id;
  S2D_Color color;
  int x;
  int y;
  int width;
  int height;
  int orig_width;
  int orig_height;
} S2D_Image;

// S2D_Sprite
typedef struct {
  S2D_Image *img;
  S2D_Color color;
  int x;
  int y;
  int width;
  int height;
  int clip_width;
  int clip_height;
  GLfloat tx1;
  GLfloat ty1;
  GLfloat tx2;
  GLfloat ty2;
  GLfloat tx3;
  GLfloat ty3;
  GLfloat tx4;
  GLfloat ty4;
} S2D_Sprite;

// S2D_Text
typedef struct {
  SDL_Surface *surface;
  GLuint texture_id;
  TTF_Font *font;
  S2D_Color color;
  const char *msg;
  int x;
  int y;
  int width;
  int height;
} S2D_Text;

// S2D_Sound
typedef struct {
  Mix_Chunk *data;
} S2D_Sound;

// S2D_Music
typedef struct {
  Mix_Music *data;
} S2D_Music;

// Simple 2D Functions /////////////////////////////////////////////////////////

/*
 * Checks if a file exists and can be accessed
 */
bool S2D_FileExists(const char *path);

/*
 * Logs standard messages to the console
 */
void S2D_Log(const char *msg, int type);

/*
 * Logs Simple 2D errors to the console, with caller and message body
 */
void S2D_Error(const char *caller, const char *msg);

/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status);

/*
 * Enable terminal colors in Windows
 */
void S2D_Windows_EnableTerminalColors();

/*
 * Initialize Simple 2D subsystems
 */
bool S2D_Init();

/*
 * Quits Simple 2D subsystems
 */
void S2D_Quit(void);

// Shapes //////////////////////////////////////////////////////////////////////

/*
 * Draw a triangle
 */
void S2D_DrawTriangle(
  GLfloat x1, GLfloat y1,
  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
  GLfloat x2, GLfloat y2,
  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
  GLfloat x3, GLfloat y3,
  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3
);

/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(
  GLfloat x1, GLfloat y1,
  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
  GLfloat x2, GLfloat y2,
  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
  GLfloat x3, GLfloat y3,
  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3,
  GLfloat x4, GLfloat y4,
  GLfloat r4, GLfloat g4, GLfloat b4, GLfloat a4
);

/*
 * Draw a line from a quad
 */
void S2D_DrawLine(
  GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
  GLfloat width,
  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3,
  GLfloat r4, GLfloat g4, GLfloat b4, GLfloat a4
);

// Image ///////////////////////////////////////////////////////////////////////

/*
 * Create an image, given a file path
 */
S2D_Image *S2D_CreateImage(const char *path);

/*
 * Draw an image
 */
void S2D_DrawImage(S2D_Image *img);

/*
 * Free an image
 */
void S2D_FreeImage(S2D_Image *img);

// Sprite //////////////////////////////////////////////////////////////////////

/*
 * Create a sprite, given an image file path
 */
S2D_Sprite *S2D_CreateSprite(const char *path);

/*
 * Clip a sprite
 */
void S2D_ClipSprite(S2D_Sprite *spr, int x, int y, int w, int h);

/*
 * Draw a sprite
 */
void S2D_DrawSprite(S2D_Sprite *spr);

/*
 * Free a sprite
 */
void S2D_FreeSprite(S2D_Sprite *spr);

// Text ////////////////////////////////////////////////////////////////////////

/*
 * Create text, given a font file path, the message, and size
 */
S2D_Text *S2D_CreateText(const char *font, const char *msg, int size);

/*
* Sets the text message
*/
void S2D_SetText(S2D_Text *txt, const char *msg);

/*
 * Draw text
 */
void S2D_DrawText(S2D_Text *txt);

/*
 * Free the text
 */
void S2D_FreeText(S2D_Text *txt);

// Sound ///////////////////////////////////////////////////////////////////////

/*
 * Create a sound, given an audio file path
 */
S2D_Sound *S2D_CreateSound(const char *path);

/*
 * Play the sound
 */
void S2D_PlaySound(S2D_Sound *sound);

/*
 * Free the sound
 */
void S2D_FreeSound(S2D_Sound *sound);

// Music ///////////////////////////////////////////////////////////////////////

/*
 * Create the music, given an audio file path
 */
S2D_Music *S2D_CreateMusic(const char *path);

/*
 * Play the music
 */
void S2D_PlayMusic(S2D_Music *music, bool loop);

/*
 * Pause the playing music
 */
void S2D_PauseMusic();

/*
 * Resume the current music
 */
void S2D_ResumeMusic();

/*
 * Stops the playing music; interrupts fader effects
 */
void S2D_StopMusic();

/*
 * Fade out the playing music
 */
void S2D_FadeOutMusic(int ms);

/*
 * Free the music
 */
void S2D_FreeMusic(S2D_Music *music);

// Input ///////////////////////////////////////////////////////////////////////

/*
 * Get the mouse coordinates relative to the viewport
 */
void S2D_GetMouseOnViewport(S2D_Window *window, int wx, int wy, int *x, int *y);

/*
 * Show or hide the cursor over the window
 */
void S2D_ShowCursor(bool toggle);

// Controllers /////////////////////////////////////////////////////////////////

/*
 * Detect controllers and joysticks
 */
void S2D_DetectControllers();

// Window //////////////////////////////////////////////////////////////////////

/*
 * Create a window
 */
S2D_Window *S2D_CreateWindow(
  const char *title, int width, int height, S2D_Update, S2D_Render, int flags
);

/*
 * Show the window
 */
int S2D_Show(S2D_Window *window);

/*
 * Close the window
 */
int S2D_Close(S2D_Window *window);

/*
 * Free all resources
 */
int S2D_FreeWindow(S2D_Window *window);

// Simple 2D OpenGL Functions //////////////////////////////////////////////////

int S2D_GL_Init(S2D_Window *window);
void S2D_GL_PrintError(char *error);
void S2D_GL_PrintContextInfo(S2D_Window *window);
void S2D_GL_StoreContextInfo(S2D_Window *window);
GLuint S2D_GL_LoadShader(GLenum type, const GLchar *shaderSrc, char *shaderName);
int S2D_GL_CheckLinked(GLuint program, char *name);
void S2D_GL_GetViewportScale(S2D_Window *window, int *w, int *h, double *scale);
void S2D_GL_SetViewport(S2D_Window *window);
void S2D_GL_CreateTexture(
  GLuint *id, GLint format,
  int w, int h,
  const GLvoid *data, GLint filter);
void S2D_GL_DrawTriangle(
  GLfloat x1, GLfloat y1,
  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
  GLfloat x2, GLfloat y2,
  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
  GLfloat x3, GLfloat y3,
  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3);
void S2D_GL_DrawImage(S2D_Image *img);
void S2D_GL_DrawSprite(S2D_Sprite *spr);
void S2D_GL_DrawText(S2D_Text *txt);
void S2D_GL_FreeTexture(GLuint *id);
void S2D_GL_Clear(S2D_Color clr);

// OpenGL & GLES Internal Functions ////////////////////////////////////////////

#if GLES
  int S2D_GLES_Init();
  void S2D_GLES_ApplyProjection(GLfloat orthoMatrix[16]);
  void S2D_GLES_DrawTriangle(
    GLfloat x1, GLfloat y1,
    GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
    GLfloat x2, GLfloat y2,
    GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
    GLfloat x3, GLfloat y3,
    GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3);
  void S2D_GLES_DrawImage(S2D_Image *img);
  void S2D_GLES_DrawSprite(S2D_Sprite *spr);
  void S2D_GLES_DrawText(S2D_Text *txt);
#else
  int S2D_GL2_Init();
  int S2D_GL3_Init();
  void S2D_GL2_ApplyProjection(int w, int h);
  void S2D_GL3_ApplyProjection(GLfloat orthoMatrix[16]);
  void S2D_GL2_DrawTriangle(
    GLfloat x1, GLfloat y1,
    GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
    GLfloat x2, GLfloat y2,
    GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
    GLfloat x3, GLfloat y3,
    GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3);
  void S2D_GL3_DrawTriangle(
    GLfloat x1, GLfloat y1,
    GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
    GLfloat x2, GLfloat y2,
    GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
    GLfloat x3, GLfloat y3,
    GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3);
  void S2D_GL2_DrawImage(S2D_Image *img);
  void S2D_GL3_DrawImage(S2D_Image *img);
  void S2D_GL2_DrawSprite(S2D_Sprite *spr);
  void S2D_GL3_DrawSprite(S2D_Sprite *spr);
  void S2D_GL2_DrawText(S2D_Text *txt);
  void S2D_GL3_DrawText(S2D_Text *txt);
#endif

#ifdef __cplusplus
}
#endif
