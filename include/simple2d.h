// simple2d.h

// If ARM, assume GLES
#ifdef __arm__
  #define GLES true
#else
  #define GLES false
#endif

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

// In simple2d.c

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
typedef void (*On_key)(const char *);
typedef void (*On_key_down)(const char *);

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

/*
 * Shared OpenGL functions
 */
void gl_print_error(char *error);
void gl_print_context_info(Window *window);
void gl_store_context_info(Window *window);
GLuint gl_load_shader(GLenum type, const GLchar *shaderSrc, char *shaderName);

/*
 * OpenGL & GLES internal functions
 */
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
Window* S2D_CreateWindow(char *title, int width, int height, Update, Render);

/*
 * Show the window
 */
int S2D_Show(Window *window);

/*
 * Close the window
 */
int S2D_Close(Window *window);

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
