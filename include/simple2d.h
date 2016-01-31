// simple2d.h

// Definitions /////////////////////////////////////////////////////////////////

#define S2D_INFO  1
#define S2D_WARN  2
#define S2D_ERROR 3

#define S2D_RESIZABLE  SDL_WINDOW_RESIZABLE
#define S2D_BORDERLESS SDL_WINDOW_BORDERLESS
#define S2D_FULLSCREEN SDL_WINDOW_FULLSCREEN_DESKTOP
#define S2D_HIGHDPI    SDL_WINDOW_ALLOW_HIGHDPI

// If ARM, assume GLES
#ifdef __arm__
  #define GLES true
#else
  #define GLES false
#endif

// Includes ////////////////////////////////////////////////////////////////////

#include <stdbool.h>
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

// Shared Data /////////////////////////////////////////////////////////////////

extern char S2D_msg[1024];  // for S2D_Log messages
extern bool S2D_GL2;        // Flag set if OpenGL 2.1
extern GLfloat S2D_GL_orthoMatrix[16];

// Type Definitions ////////////////////////////////////////////////////////////

typedef struct Color {
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
} Color;

typedef struct Mouse {
  int visible;
  int x;       // scaled positions
  int y;
  int real_x;  // actual positions
  int real_y;
} Mouse;

typedef void (*Update)(void);
typedef void (*Render)(void);
typedef void (*On_key)(const char *key);
typedef void (*On_key_down)(const char *key);
typedef void (*On_mouse)(int x, int y);
typedef void (*On_controller)(bool is_axis, int axis, int val, bool is_btn, int btn);

typedef struct Window {
  SDL_Window *sdl;
  const GLubyte *S2D_GL_VENDOR;
  const GLubyte *S2D_GL_RENDERER;
  const GLubyte *S2D_GL_VERSION;
  GLint S2D_GL_MAJOR_VERSION;
  GLint S2D_GL_MINOR_VERSION;
  const GLubyte *S2D_GL_SHADING_LANGUAGE_VERSION;
  SDL_GLContext glcontext;
  char *title;
  int width;    // actual dimentions
  int height;
  int s_width;  // scaled dimentions
  int s_height;
  int fps_cap;
  bool vsync;
  Color background;
  Update update;
  Render render;
  On_key on_key;
  On_key_down on_key_down;
  On_mouse on_mouse;
  On_controller on_controller;
  Mouse mouse;
  Uint32 frames;
  Uint32 elapsed_ms;
  Uint32 loop_ms;
  Uint32 delay_ms;
  double fps;
} Window;

typedef struct Image {
  GLuint texture_id;
  Color color;
  int x;
  int y;
  int w;
  int h;
} Image;

typedef struct Text {
  GLuint texture_id;
  Color color;
  TTF_Font *font;
  char *msg;
  int x;
  int y;
  int w;
  int h;
} Text;

typedef struct Sound {
  Mix_Chunk *data;
} Sound;

typedef struct Music {
  Mix_Music *data;
} Music;

// Simple 2D OpenGL Functions //////////////////////////////////////////////////

void S2D_GL_PrintError(char *error);
void S2D_GL_PrintContextInfo(Window *window);
void S2D_GL_StoreContextInfo(Window *window);
GLuint S2D_GL_LoadShader(GLenum type, const GLchar *shaderSrc, char *shaderName);
void S2D_GL_SetView(
  int window_width,       int window_height,
  int s2d_viewport_width, int s2d_viewport_height);
void S2D_GL_SetUpTexture(GLuint *id, GLint format, int w, int h, const GLvoid *data, GLint filter);
void S2D_GL_DrawTriangle(
  GLfloat x1,  GLfloat y1,
  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
  GLfloat x2,  GLfloat y2,
  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
  GLfloat x3,  GLfloat y3,
  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
void S2D_GL_DrawImage(Image img);
void S2D_GL_DrawText(Text txt);
void S2D_GL_FreeTexture(GLuint *id);
void S2D_GL_Clear(Color clr);

// OpenGL & GLES Internal Functions ////////////////////////////////////////////

#if GLES
  void gles_hello();
  int gles_check_linked(GLuint program, char *name);
  int gles_init(int width, int height, int s_width, int s_height);
  void gles_draw_triangle(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void gles_draw_image(Image img);
  void gles_draw_text(Text txt);
#else
  void gl2_hello();
  void gl3_hello();
  int gl3_check_linked(GLuint program);
  int gl2_init(int width, int height);
  int gl3_init(int width, int height);
  void gl3_set_view(
    int window_width,       int window_height,
    int s2d_viewport_width, int s2d_viewport_height);
  void gl2_draw_triangle(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void gl3_draw_triangle(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void gl2_draw_image(Image img);
  void gl3_draw_image(Image img);
  void gl2_draw_text(Text txt);
  void gl3_draw_text(Text txt);
#endif

// S2D Functions ///////////////////////////////////////////////////////////////

/*
 * Logs standard messages to the console
 */
void S2D_Log(char *msg, int type);

/*
 * Logs Simple 2D errors to the console, with caller and message body
 */
void S2D_Error(char *caller, const char *msg);

/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status);

/*
 * Print info about the current OpenGL context
 */
void print_gl_context();

/*
 * Create the window
 */
Window* S2D_CreateWindow(
  char *title, int width, int height, Update, Render, int flags
);

/*
 * Show the window
 */
int S2D_Show(Window *window);

/*
 * Close the window
 */
int S2D_Close();

/*
 * Free all resources
 */
int S2D_FreeWindow(Window *window);

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
Image S2D_CreateImage(char *path);

/*
 * Draw an image
 */
void S2D_DrawImage(Image img);

/*
 * Free an image
 */
void S2D_FreeImage(Image img);

/*
 * Create text
 */
Text S2D_CreateText(char *font, char *msg, int size);

/*
* Sets the text message
*/
void S2D_SetText(Text *txt, char *msg);

/*
 * Draw text
 */
void S2D_DrawText(Text txt);

/*
 * Free the text
 */
void S2D_FreeText(Text txt);

/*
 * Create a sound
 */
Sound S2D_CreateSound(char *path);

/*
 * Play the sound
 */
void S2D_PlaySound(Sound sound);

/*
 * Free the sound
 */
void S2D_FreeSound(Sound sound);

/*
 * Create music
 */
Music S2D_CreateMusic(char *path);

/*
 * Play the music
 */
void S2D_PlayMusic(Music music, int times);

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
void S2D_FreeMusic(Music music);
