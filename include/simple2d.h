// simple2d.h

// Includes ////////////////////////////////////////////////////////////////////

#include <stdbool.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#if GLES
  #include <SDL2/SDL_opengles2.h>
#else
  #define GL_GLEXT_PROTOTYPES 1
  #include <SDL2/SDL_opengl.h>
#endif

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

// Definitions /////////////////////////////////////////////////////////////////

// If ARM, assume GLES
#ifdef __arm__
  #define GLES true
#else
  #define GLES false
#endif

// Messages
#define S2D_INFO  1
#define S2D_WARN  2
#define S2D_ERROR 3

// Window attributes
#define S2D_RESIZABLE  SDL_WINDOW_RESIZABLE
#define S2D_BORDERLESS SDL_WINDOW_BORDERLESS
#define S2D_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP
#define S2D_HIGHDPI    SDL_WINDOW_ALLOW_HIGHDPI

// Viewport scaling
#define S2D_FIXED   1
#define S2D_SCALE   2
#define S2D_STRETCH 3

// Shared Data /////////////////////////////////////////////////////////////////

extern char S2D_msg[1024];    // for S2D_Log messages
extern bool S2D_diagnostics;  // Flag whether to print diagnostics with S2D_Log

// Type Definitions ////////////////////////////////////////////////////////////

typedef void (*S2D_Update)(void);
typedef void (*S2D_Render)(void);
typedef void (*S2D_On_Key)(const char *key);
typedef void (*S2D_On_Key_Down)(const char *key);
typedef void (*S2D_On_Mouse)(int x, int y);
typedef void (*S2D_On_Controller)(bool is_axis, int axis, int val, bool is_btn, int btn);

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
  int x;       // scaled positions
  int y;
  int real_x;  // actual positions
  int real_y;
} S2D_Mouse;

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
  bool close;
  const char *title;
  int orig_width;  // original dimentions
  int orig_height;
  int width;       // actual dimentions
  int height;
  int s_width;     // scaled dimentions
  int s_height;
  int viewport;
  int fps_cap;
  bool vsync;
  S2D_Color background;
  S2D_Update update;
  S2D_Render render;
  int flags;
  S2D_On_Key on_key;
  S2D_On_Key_Down on_key_down;
  S2D_On_Mouse on_mouse;
  S2D_On_Controller on_controller;
  S2D_Mouse mouse;
  Uint32 frames;
  Uint32 elapsed_ms;
  Uint32 loop_ms;
  Uint32 delay_ms;
  double fps;
} S2D_Window;

// S2D_Image
typedef struct {
  SDL_Surface *surface;
  int format;
  GLuint texture_id;
  S2D_Color color;
  int x;
  int y;
  int w;
  int h;
} S2D_Image;

// S2D_Text
typedef struct {
  SDL_Surface *surface;
  GLuint texture_id;
  TTF_Font *font;
  S2D_Color color;
  const char *msg;
  int x;
  int y;
  int w;
  int h;
} S2D_Text;

// S2D_Sound
typedef struct {
  Mix_Chunk *data;
} S2D_Sound;

// S2D_Music
typedef struct {
  Mix_Music *data;
} S2D_Music;

// Simple 2D OpenGL Functions //////////////////////////////////////////////////

int S2D_GL_Init(S2D_Window *window);
void S2D_GL_PrintError(char *error);
void S2D_GL_PrintContextInfo(S2D_Window *window);
void S2D_GL_StoreContextInfo(S2D_Window *window);
GLuint S2D_GL_LoadShader(GLenum type, const GLchar *shaderSrc, char *shaderName);
int S2D_GL_CheckLinked(GLuint program, char *name);
void S2D_GL_SetViewport(S2D_Window *window);
void S2D_GL_SetUpTexture(
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
  void S2D_GL2_DrawText(S2D_Text *txt);
  void S2D_GL3_DrawText(S2D_Text *txt);
#endif

// S2D Functions ///////////////////////////////////////////////////////////////

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
 * Create a window structure and initiate subsystems
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

/*
 * Quits S2D subsystems
 */
void S2D_Quit();

/*
 * Draw triangle
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
 * Draw quad
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

/*
 * Create an image
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

/*
 * Create text
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

/*
 * Create a sound
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

/*
 * Create music
 */
S2D_Music *S2D_CreateMusic(const char *path);

/*
 * Play the music
 */
void S2D_PlayMusic(S2D_Music *music, int times);

/*
 * Pause the playing music
 */
void S2D_PauseMusic();

/*
 * Resume the current music
 */
void S2D_ResumeMusic();

/*
 * Stops the playing music
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
