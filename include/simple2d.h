// simple2d.h

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

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

// Events
#define S2D_KEYDOWN 1  // key is pressed
#define S2D_KEY     2  // key is held down
#define S2D_KEYUP   3  // key is released

// Internal Shared Data ////////////////////////////////////////////////////////

extern char S2D_msg[1024];    // for S2D_Log messages
extern bool S2D_diagnostics;  // flag for whether to print diagnostics with S2D_Log

// Type Definitions ////////////////////////////////////////////////////////////

typedef int S2D_Event;
typedef void (*S2D_Update)();
typedef void (*S2D_Render)();
typedef void (*S2D_On_Key)(S2D_Event e, const char *key);
typedef void (*S2D_On_Mouse)(int x, int y);
typedef void (*S2D_On_Controller)(
  int which, bool is_axis, int axis, int val, bool is_btn, int btn, bool pressed
);

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
  S2D_Viewport viewport;
  S2D_Update update;
  S2D_Render render;
  int flags;
  S2D_Mouse mouse;
  S2D_On_Key on_key;
  S2D_On_Mouse on_mouse;
  S2D_On_Controller on_controller;
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
  int x;
  int y;
  int width;
  int height;
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
  GLfloat x1,  GLfloat y1,
  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
  GLfloat x2,  GLfloat y2,
  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
  GLfloat x3,  GLfloat y3,
  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a
);

/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(
  GLfloat x1,  GLfloat y1,
  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
  GLfloat x2,  GLfloat y2,
  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
  GLfloat x3,  GLfloat y3,
  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a,
  GLfloat x4,  GLfloat y4,
  GLfloat c4r, GLfloat c4g, GLfloat c4b, GLfloat c4a
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
  GLfloat x1,  GLfloat y1,
  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
  GLfloat x2,  GLfloat y2,
  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
  GLfloat x3,  GLfloat y3,
  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
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
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void S2D_GLES_DrawImage(S2D_Image *img);
  void S2D_GLES_DrawSprite(S2D_Sprite *spr);
  void S2D_GLES_DrawText(S2D_Text *txt);
#else
  int S2D_GL2_Init();
  int S2D_GL3_Init();
  void S2D_GL2_ApplyProjection(int w, int h);
  void S2D_GL3_ApplyProjection(GLfloat orthoMatrix[16]);
  void S2D_GL2_DrawTriangle(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void S2D_GL3_DrawTriangle(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void S2D_GL2_DrawImage(S2D_Image *img);
  void S2D_GL3_DrawImage(S2D_Image *img);
  void S2D_GL2_DrawSprite(S2D_Sprite *spr);
  void S2D_GL3_DrawSprite(S2D_Sprite *spr);
  void S2D_GL2_DrawText(S2D_Text *txt);
  void S2D_GL3_DrawText(S2D_Text *txt);
#endif
