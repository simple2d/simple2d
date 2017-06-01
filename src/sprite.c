// sprite.c

#include "../include/simple2d.h"


/*
 * Create a sprite, given an image file path
 */
S2D_Sprite *S2D_CreateSprite(const char *path) {

  // Check if image file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateSprite", "Sprite image file not found");
    return NULL;
  }

  S2D_Sprite *spr = (S2D_Sprite *) malloc(sizeof(S2D_Sprite));
  spr->img = S2D_CreateImage(path);

  spr->x = 0;
  spr->y = 0;
  spr->color.r = 1.f;
  spr->color.g = 1.f;
  spr->color.b = 1.f;
  spr->color.a = 1.f;
  spr->width  = spr->img->width;
  spr->height = spr->img->height;
  spr->clip_width  = spr->img->width;
  spr->clip_height = spr->img->height;

  spr->tx1 = 0.f;
  spr->ty1 = 0.f;
  spr->tx2 = 1.f;
  spr->ty2 = 0.f;
  spr->tx3 = 1.f;
  spr->ty3 = 1.f;
  spr->tx4 = 0.f;
  spr->ty4 = 1.f;

  return spr;
}


/*
 * Clip a sprite
 */
void S2D_ClipSprite(S2D_Sprite *spr, int x, int y, int w, int h) {
  if (!spr) return;

  // Calculate ratios
  // rw = ratio width; rh = ratio height
  double rw = w / (double)spr->img->width;
  double rh = h / (double)spr->img->height;

  // Apply ratios to x, y coordinates
  // cx = crop x coord; cy = crop y coord
  double cx = x * rw;
  double cy = y * rh;

  // Convert given width, height to doubles
  // cw = crop width; ch = crop height
  double cw = (double)w;
  double ch = (double)h;

  // Apply ratio to texture width and height
  // tw = texture width; th = texture height
  double tw = rw * w;
  double th = rh * h;

  // Calculate and store sprite texture values

  spr->tx1 =  cx       / cw;
  spr->ty1 =  cy       / ch;

  spr->tx2 = (cx + tw) / cw;
  spr->ty2 =  cy       / ch;

  spr->tx3 = (cx + tw) / cw;
  spr->ty3 = (cy + th) / ch;

  spr->tx4 =  cx       / cw;
  spr->ty4 = (cy + th) / ch;

  // Store the sprite dimensions
  spr->width  = (spr->width  / (double)spr->clip_width ) * w;
  spr->height = (spr->height / (double)spr->clip_height) * h;
  spr->clip_width  = w;
  spr->clip_height = h;
}


/*
 * Draw a sprite
 */
void S2D_DrawSprite(S2D_Sprite *spr) {
  if (!spr) return;

  if (spr->img->texture_id == 0) {
    S2D_GL_CreateTexture(&spr->img->texture_id, spr->img->format,
                         spr->img->width, spr->img->height,
                         spr->img->surface->pixels, GL_NEAREST);
    SDL_FreeSurface(spr->img->surface);
  }

  S2D_GL_DrawSprite(spr);
}


/*
 * Free a sprite
 */
void S2D_FreeSprite(S2D_Sprite *spr) {
  if (!spr) return;
  S2D_FreeImage(spr->img);
  free(spr);
}
