// window.c

#include "../include/simple2d.h"


/*
 * Map window to renderer coordinates
 */
void S2D_WindowToRendererCoordinates(SDL_Vertex *vertices, int count) {
  float scale = s2d_app.window->display_scale;
  for (int i = 0; i < count; i++) {
    vertices[i].position.x *= scale;
    vertices[i].position.y *= scale;
  }
}


/*
 * Map window to renderer coordinates for a rectangle
 */
void S2D_WindowToRendererCoordinatesRect(SDL_FRect *rect) {
  float scale = s2d_app.window->display_scale;
  rect->x *= scale;
  rect->y *= scale;
  rect->w *= scale;
  rect->h *= scale;
}


/*
 * Create and initialize the Simple 2D window structure
 */
S2D_Window *S2D_CreateWindow(const char *title, int width, int height) {
  S2D_Window *window = (S2D_Window *) calloc(1, sizeof(S2D_Window));
  window->title = title;
  window->width = width;
  window->height = height;
  window->flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
  window->background.r = 0.0;
  window->background.g = 0.0;
  window->background.b = 0.0;
  window->background.a = 1.0;
  window->frames = 0;
  window->close = false;

  // Get the current display mode
  int display_count = 0;
  SDL_DisplayID *display_id = SDL_GetDisplays(&display_count);
  if (!display_id || display_count < 1) {
    S2D_Error("SDL_GetDisplays", SDL_GetError());
    free(window);
    return NULL;
  }
  const SDL_DisplayMode *mode = SDL_GetCurrentDisplayMode(display_id[0]);
  if (mode == NULL) {
    S2D_Error("SDL_GetCurrentDisplayMode", SDL_GetError());
    SDL_free(display_id);
    return NULL;
  }
  float content_scale = SDL_GetDisplayContentScale(display_id[0]);
  float display_scale = mode->pixel_density * content_scale;

  S2D_Log(S2D_INFO, "Display (%u of %d)", display_id[0], display_count);
  S2D_Log(S2D_INFO, "  width: %d", mode->w);
  S2D_Log(S2D_INFO, "  height: %d", mode->h);
  S2D_Log(S2D_INFO, "  pixel_density: %.2f", mode->pixel_density);
  S2D_Log(S2D_INFO, "  content_scale: %.2f", content_scale);
  S2D_Log(S2D_INFO, "  display_scale: %.2f", display_scale);
  S2D_Log(S2D_INFO, "  refresh_rate: %.2f", mode->refresh_rate);

  // Set the main callback rate to the display's refresh rate
  // VSync will cap FPS, but this is needed if window is occluded so it doesn't run infinitely fast
  char refresh_rate_str[16];
  snprintf(refresh_rate_str, sizeof(refresh_rate_str), "%d", (int)mode->refresh_rate);
  SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, refresh_rate_str);

  window->display_width = mode->w;
  window->display_height = mode->h;
  window->display_scale = display_scale;
  window->display_content_scale = content_scale;
  window->display_refresh_rate = mode->refresh_rate;

  SDL_free(display_id);

  s2d_app.window = window;
  return window;
}


/*
 * Show the window by creating the SDL window and renderer, and enabling VSync
 */
bool S2D_ShowWindow() {
  // Scale window dimensions by content_scale so the window appears at the
  // correct logical size on all platforms. On macOS, content_scale is 1.0
  // (window coords are already in points). On Windows, content_scale matches
  // the DPI scaling (e.g. 2.0 at 200%), and window coords are physical pixels.
  float cs = s2d_app.window->display_content_scale;
  int win_w = (int)(s2d_app.window->width * cs);
  int win_h = (int)(s2d_app.window->height * cs);

  if (!SDL_CreateWindowAndRenderer(
        s2d_app.window->title, win_w, win_h,
        s2d_app.window->flags, &s2d_app.window->sdl_window, &s2d_app.window->sdl_renderer)) {
    S2D_Error("S2D_ShowWindow / SDL_CreateWindowAndRenderer", SDL_GetError());
    return false;
  }

  SDL_SetRenderDrawBlendMode(s2d_app.window->sdl_renderer, SDL_BLENDMODE_BLEND);

  if (!SDL_SetRenderVSync(s2d_app.window->sdl_renderer, 1)) {
    S2D_Error("S2D_ShowWindow / SDL_SetRenderVSync", SDL_GetError());
    return false;
  }

  return true;
}


/*
 * Free the Simple 2D window structure and associated resources
 */
void S2D_FreeWindow(S2D_Window *window) {
  if (!window) return;

  if (window->sdl_renderer) {
    SDL_DestroyRenderer(window->sdl_renderer);
    window->sdl_renderer = NULL;
  }

  if (window->sdl_window) {
    SDL_DestroyWindow(window->sdl_window);
    window->sdl_window = NULL;
  }

  free(window);
}


/*
 * Close the window
 */
void S2D_Close() {
  s2d_app.quit = true;
  S2D_Log(S2D_INFO, "Closing window");
}
