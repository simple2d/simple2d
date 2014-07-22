// simple2d.h
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

typedef void (*Update)(void);
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
} Text;

typedef struct Sound {
  Mix_Chunk *wave;
} Sound;

/*
 * Show the window
 */
extern Window* S2D_CreateWindow(
  char *title, int width, int height,
  int fps_cap, bool vsync, Update, On_key, Key_down
);

/*
 * Show the window
 */
extern int S2D_Show(Window*);

/*
 * Draw triangle
 */
extern void S2D_DrawTriangle(
  double x1,  double y1,
  double c1r, double c1g, double c1b, double c1a,
  double x2,  double y2,
  double c2r, double c2g, double c2b, double c2a,
  double x3,  double y3,
  double c3r, double c3g, double c3b, double c3a
);

/*
 * Draw quad
 */
extern void S2D_DrawQuad(
  double x1,  double y1,
  double c1r, double c1g, double c1b, double c1a,
  double x2,  double y2,
  double c2r, double c2g, double c2b, double c2a,
  double x3,  double y3,
  double c3r, double c3g, double c3b, double c3a,
  double x4,  double y4,
  double c4r, double c4g, double c4b, double c4a
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
extern Text S2D_CreateText(char *path);

/*
 * Draw text
 */
extern void S2D_DrawText(Text *text, int x, int y);

/*
 * Create a sound
 */
extern Text S2D_CreateText(char *path);
