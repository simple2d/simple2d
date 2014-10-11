// simple2d.h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef void (*Update)(void);
typedef void (*Render)(void);
typedef void (*On_key)(const char *);
typedef void (*Key_down)(const char *);

typedef struct Window {
  SDL_Window *sdl_window;
  SDL_Renderer *renderer;
  SDL_GLContext glcontext;
  char *title;
  int width;
  int height;
  int fps_cap;
  bool vsync;
  Update update;
  Render render;
  On_key on_key;
  Key_down key_down;
  int cursor_x;
  int cursor_y;
  Uint32 frames;
  Uint32 total_ms;
  Uint32 loop_ms;
  Uint32 delay_ms;
  double fps;
} Window;

typedef struct Image {
  SDL_Surface *surface;
  SDL_Texture *texture;
} Image;

typedef struct Text {
  TTF_Font *font;
  SDL_Surface *surface;
  SDL_Texture *texture;
  char *msg;
} Text;

typedef struct Sound {
  Mix_Chunk *wave;
} Sound;

/*
 * Show the window
 */
extern Window* S2D_CreateWindow(
  char *title, int width, int height,
  int fps_cap, bool vsync, Update, Render, On_key, Key_down
);

/*
 * Show the window
 */
extern int S2D_Show(Window*);

/*
 * Draw triangle
 */
extern void S2D_DrawTriangle(
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
extern void S2D_DrawQuad(
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
extern Image* S2D_CreateImage(Window *window, char *path);

/*
 * Draw an image
 */
extern void S2D_DrawImage(Image *img, int x, int y);

/*
 * Free an image
 */
extern void S2D_FreeImage(Image *img);

/*
 * Create text
 */
extern Text* S2D_CreateText(Window *window, char *font, char *msg, int size);

/*
 * Draw text
 */
extern void S2D_DrawText(Text *text, int x, int y);

/*
 * Free the text
 */
extern void S2D_FreeText(Text *text);

/*
 * Create a sound
 */
extern Sound* S2D_CreateSound(char *path);

/*
 * Play the sound
 */
extern void S2D_PlaySound(Sound *sound);

/*
 * Free the sound
 */
extern void S2D_FreeSound(Sound *sound);
