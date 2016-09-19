// sprite.c

#include "../include/simple2d.h"


/*
 * Create a sprite
 * Params: path = image file path
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
  spr->w = spr->img->w;
  spr->h = spr->img->h;
  
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
  double rw = w / (double)spr->img->w;
  double rh = h / (double)spr->img->h;
  
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
  
  // Store the sprite width and height
  spr->w = w;
  spr->h = h;
}


/*
 * Draw a sprite
 */
void S2D_DrawSprite(S2D_Sprite *spr) {
  if (!spr) return;
  
  if (spr->img->texture_id == 0) {
    S2D_GL_SetUpTexture(&spr->img->texture_id, spr->img->format,
                        spr->img->w, spr->img->h,
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
