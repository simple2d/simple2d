// text.c

#include "../include/simple2d.h"


/*
 * Create text, given a font file path, the message, and size
 */
S2D_Text *S2D_CreateText(const char *font, const char *msg, int size) {
  if (!s2d_app.window) {
    S2D_Error("S2D_CreateText", "A window must be created with `S2D_CreateWindow` before calling `S2D_CreateText`");
    return NULL;
  }

  // Check if font file exists
  if (!S2D_FileExists(font)) {
    S2D_Error("S2D_CreateText", "Font file `%s` not found", font);
    return NULL;
  }

  // Allocate the text structure
  S2D_Text *txt = (S2D_Text *) malloc(sizeof(S2D_Text));
  if (!txt) {
    S2D_Error("S2D_CreateText", "Out of memory!");
    return NULL;
  }

  // Don't allow `msg` to be an empty string or NULL
  if (msg == NULL || strlen(msg) == 0) msg = " ";

  // Initialize values
  txt->msg = strdup(msg);
  txt->texture = NULL;
  txt->font = strdup(font);
  txt->x = 0;
  txt->y = 0;
  txt->color.r = 1.f;
  txt->color.g = 1.f;
  txt->color.b = 1.f;
  txt->color.a = 1.f;
  txt->rotate = 0;
  txt->rx = 0;
  txt->ry = 0;

  // Open the font
  txt->font_data = TTF_OpenFont(font, (int)(size * s2d_app.window->display_scale));
  if (!txt->font_data) {
    S2D_Error("TTF_OpenFont", SDL_GetError());
    free((void*)txt->font);
    free(txt->msg);
    free(txt);
    return NULL;
  }

  // Create the font surface
  SDL_Color color = { 255, 255, 255 };
  txt->surface = TTF_RenderText_Blended(txt->font_data, msg, 0, color);
  if (!txt->surface) {
    S2D_Error("TTF_RenderText_Blended", SDL_GetError());
    TTF_CloseFont(txt->font_data);
    free((void*)txt->font);
    free(txt->msg);
    free(txt);
    return NULL;
  }

  // Save the width and height of the text
  if (!TTF_GetStringSize(txt->font_data, txt->msg, strlen(txt->msg), &txt->width, &txt->height)) {
    S2D_Error("TTF_GetStringSize", SDL_GetError());
    SDL_DestroySurface(txt->surface);
    TTF_CloseFont(txt->font_data);
    free((void*)txt->font);
    free(txt->msg);
    free(txt);
    return NULL;
  }
  txt->width  /= s2d_app.window->display_scale;
  txt->height /= s2d_app.window->display_scale;

  return txt;
}


/*
 * Set the text message
 */
void S2D_SetText(S2D_Text *txt, const char *msg, ...) {
  if (!txt) return;

  // Don't allow `msg` to be an empty string or NULL
  if (msg == NULL || strlen(msg) == 0) msg = " ";

  // Format and store new text string
  va_list args;
  va_start(args, msg);
  free((void*)txt->msg);
  vasprintf(&txt->msg, msg, args);
  va_end(args);

  // Save the width and height of the text
  if (!TTF_GetStringSize(txt->font_data, txt->msg, strlen(txt->msg), &txt->width, &txt->height)) {
    S2D_Error("TTF_GetStringSize", SDL_GetError());
  }
  txt->width  /= s2d_app.window->display_scale;
  txt->height /= s2d_app.window->display_scale;

  // Delete the current texture so a new one can be generated
  if (txt->surface) SDL_DestroySurface(txt->surface);
  if (txt->texture) SDL_DestroyTexture(txt->texture);
  txt->texture = NULL;

  // Create the font surface
  SDL_Color color = { 255, 255, 255 };
  txt->surface = TTF_RenderText_Blended(txt->font_data, txt->msg, 0, color);
  if (!txt->surface) {
    S2D_Error("TTF_RenderText_Blended", SDL_GetError());
  }
}


/*
 * Rotate text
 */
void S2D_RotateText(S2D_Text *txt, float angle, int position) {

  S2D_Point p = S2D_GetRectRotationPoint(
    txt->x, txt->y, txt->width, txt->height, position
  );

  txt->rotate = angle;
  txt->rx = p.x;
  txt->ry = p.y;
}


/*
 * Draw text
 */
void S2D_DrawText(S2D_Text *txt) {
  if (!txt) return;

  if (txt->texture == NULL) {
    txt->texture = SDL_CreateTextureFromSurface(s2d_app.window->sdl_renderer, txt->surface);
    if (!txt->texture) {
      S2D_Error("SDL_CreateTextureFromSurface", SDL_GetError());
      return;
    }
    SDL_SetTextureBlendMode(txt->texture, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(txt->surface);
    txt->surface = NULL;
  }

  SDL_FRect dst_rect = {
    txt->x,
    txt->y,
    txt->width,
    txt->height
  };

  S2D_WindowToRendererCoordinatesRect(&dst_rect);
  SDL_SetTextureColorModFloat(txt->texture, txt->color.r, txt->color.g, txt->color.b);
  SDL_SetTextureAlphaModFloat(txt->texture, txt->color.a);

  float scale = s2d_app.window->display_scale;
  SDL_FPoint center = {
    (txt->rx - txt->x) * scale,
    (txt->ry - txt->y) * scale
  };
  SDL_RenderTextureRotated(
    s2d_app.window->sdl_renderer, txt->texture, NULL, &dst_rect, txt->rotate, &center, SDL_FLIP_NONE
  );
}


/*
 * Free the text
 */
void S2D_FreeText(S2D_Text *txt) {
  if (!txt) return;
  free((void*)txt->font);
  free((void*)txt->msg);
  if (txt->surface) SDL_DestroySurface(txt->surface);
  if (txt->texture) SDL_DestroyTexture(txt->texture);
  TTF_CloseFont(txt->font_data);
  free(txt);
}
