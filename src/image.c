// image.c

#include "../include/simple2d.h"


/*
 * Create an image, given a file path
 */
S2D_Image *S2D_CreateImage(const char *path) {

  // Check if image file exists
  if (!S2D_FileExists(path)) {
    S2D_Error("S2D_CreateImage", "Image file `%s` not found", path);
    return NULL;
  }

  // Allocate the image structure
  S2D_Image *img = (S2D_Image *) malloc(sizeof(S2D_Image));
  if (!img) {
    S2D_Error("S2D_CreateImage", "Out of memory!");
    return NULL;
  }

  // Load image from file as SDL_Surface
  img->surface = IMG_Load(path);
  if (!img->surface) {
    S2D_Error("IMG_Load", SDL_GetError());
    free(img);
    return NULL;
  }

  // Initialize values
  img->path = strdup(path);
  img->texture = NULL;
  img->x = 0;
  img->y = 0;
  img->color.r = 1.f;
  img->color.g = 1.f;
  img->color.b = 1.f;
  img->color.a = 1.f;
  img->orig_width  = img->surface->w;
  img->orig_height = img->surface->h;
  img->width  = img->orig_width;
  img->height = img->orig_height;
  img->clipped = false;
  img->clip_x = 0;
  img->clip_y = 0;
  img->clip_width = img->width;
  img->clip_height = img->height;
  img->rotate = 0.0;
  img->rx = 0.0;
  img->ry = 0.0;

  return img;
}


/*
 * Rotate an image
 */
void S2D_RotateImage(S2D_Image *img, float angle, int position) {
  float rect_x = img->x;
  float rect_y = img->y;
  float rect_w = img->width;
  float rect_h = img->height;

  // If clipped, adjust the rotation center to be relative to the clipped region
  if (img->clipped) {
    // The drawn rectangle is the clipped region, scaled to img->width/height
    // The rotation point should be within the drawn rectangle
    // So, use the drawn rectangle's size and position
    rect_w = img->width * ((float)img->clip_width / img->orig_width);
    rect_h = img->height * ((float)img->clip_height / img->orig_height);
  }

  S2D_Point p = S2D_GetRectRotationPoint(
    rect_x, rect_y, rect_w, rect_h, position
  );

  img->rotate = angle;
  img->rx = p.x;
  img->ry = p.y;
}


/*
 * Set the clipping rectangle for an image
 */
void S2D_ClipImage(S2D_Image *img, int x, int y, int width, int height) {
  if (!img) return;
  img->clipped = true;
  img->clip_x = x;
  img->clip_y = y;
  img->clip_width = width;
  img->clip_height = height;
}


/*
 * Remove the clipping rectangle from an image
 */
void S2D_UnclipImage(S2D_Image *img) {
  if (!img) return;
  img->clipped = false;
  img->clip_x = 0;
  img->clip_y = 0;
  img->clip_width  = img->orig_width;
  img->clip_height = img->orig_height;
}


/*
 * Draw an image
 */
void S2D_DrawImage(S2D_Image *img) {
  if (!img) return;

  if (img->texture == NULL) {
    img->texture = SDL_CreateTextureFromSurface(s2d_app.window->sdl_renderer, img->surface);
    if (!img->texture) {
      S2D_Error("SDL_CreateTextureFromSurface", SDL_GetError());
      return;
    }
    SDL_SetTextureBlendMode(img->texture, SDL_BLENDMODE_BLEND);
    SDL_DestroySurface(img->surface);
    img->surface = NULL;
  }

  SDL_FRect dst_rect = {
    img->x,
    img->y,
    img->width,
    img->height
  };

  SDL_FRect clip_rect;
  SDL_FRect *src_rect = NULL;

  if (img->clipped) {
    // Clamp clip_width and clip_height to not exceed the original image size
    int clipped_w = img->clip_width  > img->orig_width  ? img->orig_width  : img->clip_width;
    int clipped_h = img->clip_height > img->orig_height ? img->orig_height : img->clip_height;

    clip_rect.x = img->clip_x;
    clip_rect.y = img->clip_y;
    clip_rect.w = clipped_w;
    clip_rect.h = clipped_h;
    src_rect = &clip_rect;

    // Adjust dst_rect size to match the clipped region
    dst_rect.w = img->width * ((float)clipped_w / img->orig_width);
    dst_rect.h = img->height * ((float)clipped_h / img->orig_height);
  }

  S2D_WindowToRendererCoordinatesRect(&dst_rect);
  SDL_SetTextureColorModFloat(img->texture, img->color.r, img->color.g, img->color.b);
  SDL_SetTextureAlphaModFloat(img->texture, img->color.a);

  float scale = s2d_app.window->display_scale;
  SDL_FPoint center = {
    (img->rx - img->x) * scale,
    (img->ry - img->y) * scale
  };
  SDL_RenderTextureRotated(
    s2d_app.window->sdl_renderer, img->texture, src_rect, &dst_rect, img->rotate, &center, SDL_FLIP_NONE
  );
}


/*
 * Free an image
 */
void S2D_FreeImage(S2D_Image *img) {
  if (!img) return;
  free((void*)img->path);
  if (img->surface) SDL_DestroySurface(img->surface);
  if (img->texture) SDL_DestroyTexture(img->texture);
  free(img);
}
