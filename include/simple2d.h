// simple2d.h

// If ARM, assume GLES
#ifdef __arm__
  #define GLES true
#else
  #define GLES false
#endif

#include <stdbool.h>
#include <SDL2/SDL.h>

// Do we need this?
//   #define GLEW_STATIC
//   #include <GL/glew.h>

#if GLES
  #include <SDL2/SDL_opengles2.h>
#else
  #include <SDL2/SDL_opengl.h>
#endif

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef struct Color {
  GLfloat r;
  GLfloat g;
  GLfloat b;
  GLfloat a;
} Color;

typedef void (*Update)(void);
typedef void (*Render)(void);
typedef void (*On_key)(const char *);
typedef void (*Key_down)(const char *);

typedef struct Window {
  SDL_Window *sdl;
  SDL_Renderer *renderer;
  SDL_GLContext glcontext;
  char *title;
  int width;     // actual window width
  int height;    // actual window height
  int s_width;   // scaled window width
  int s_height;  // scaled window height
  int fps_cap;
  bool vsync;
  Update update;
  Render render;
  On_key on_key;
  Key_down key_down;
  int cursor_x;    // scaled cursor x position
  int cursor_y;    // scaled cursor y position
  int a_cursor_x;  // actual cursor x position
  int a_cursor_y;  // actual cursor y position
  Uint32 frames;
  Uint32 elapsed_ms;
  Uint32 loop_ms;
  Uint32 delay_ms;
  double fps;
  Color background;
} Window;

typedef struct Image {
  SDL_Texture *texture;
  int x;
  int y;
} Image;

typedef struct Text {
  TTF_Font *font;
  SDL_Texture *texture;
  char *msg;
  int x;
  int y;
  int w;
  int h;
  Color color;
} Text;

typedef struct Sound {
  Mix_Chunk *wave;
} Sound;

/*
 * OpenGL & ES internal functions
 */
#if GLES
  void hello_gles();
  int init_gles(int width, int height);
  void draw_triangle_gles(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void draw_image_gles(Image img);
  void draw_text_gles(Text txt);
#else
  void hello_gl();
  int init_gl(int width, int height);
  void draw_triangle_gl(
    GLfloat x1,  GLfloat y1,
    GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
    GLfloat x2,  GLfloat y2,
    GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
    GLfloat x3,  GLfloat y3,
    GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a);
  void draw_image_gl(Image img);
  void draw_text_gl(Text txt);
#endif

/*
* Print SDL errors
*/
void sdl_error(char *error);

/*
 * Show the window
 */
Window* S2D_CreateWindow(
  char *title, int width, int height,
  Update, Render, On_key, Key_down
);

/*
 * Show the window
 */
int S2D_Show(Window *window);

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
Image S2D_CreateImage(Window *window, char *path);

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
Text S2D_CreateText(Window *window, char *font, char *msg, int size);

/*
* Sets the text message
*/
void S2D_SetText(Window *window, Text *txt, char *msg);

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
