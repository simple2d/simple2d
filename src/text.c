// text.c

#include "../include/simple2d.h"


/*
 * Create text, given a font file path, the message, and size
 */
S2D_Text *S2D_CreateText(const char *font, const char *msg, int size) {
  S2D_Init();

  // Check if font file exists
  if (!S2D_FileExists(font)) {
    S2D_Error("S2D_CreateText", "Font file not found");
    return NULL;
  }

  // `msg` cannot be an empty string or NULL for TTF_SizeText
  if (msg == NULL || strlen(msg) == 0) msg = " ";

  // Allocate the text structure
  S2D_Text *txt = (S2D_Text *) malloc(sizeof(S2D_Text));
  if (!txt) {
    S2D_Error("S2D_CreateText", "Out of memory!");
    return NULL;
  }

  // Open the font
  txt->font_data = TTF_OpenFont(font, size);
  if (!txt->font_data) {
    S2D_Error("TTF_OpenFont", TTF_GetError());
    free(txt);
    return NULL;
  }

  // Initialize values
  txt->font = font;
  txt->msg = (char *) malloc(strlen(msg) + 1 * sizeof(char));
  strcpy(txt->msg, msg);
  txt->x = 0;
  txt->y = 0;
  txt->color.r = 1.f;
  txt->color.g = 1.f;
  txt->color.b = 1.f;
  txt->color.a = 1.f;
  txt->rotate = 0;
  txt->rx = 0;
  txt->ry = 0;
  txt->texture_id = 0;

  // Save the width and height of the text
  TTF_SizeText(txt->font_data, txt->msg, &txt->width, &txt->height);

  return txt;
}


/*
 * Set the text message
 */
void S2D_SetText(S2D_Text *txt, const char *msg, ...) {
  if (!txt) return;

  // `msg` cannot be an empty string or NULL for TTF_SizeText
  if (msg == NULL || strlen(msg) == 0) msg = " ";

  // Format and store new text string
  va_list args;
  va_start(args, msg);
  free(txt->msg);
  vasprintf(&txt->msg, msg, args);
  va_end(args);

  // Save the width and height of the text
  TTF_SizeText(txt->font_data, txt->msg, &txt->width, &txt->height);

  // Delete the current texture so a new one can be generated
  S2D_GL_FreeTexture(&txt->texture_id);
}


/*
 * Rotate text
 */
void S2D_RotateText(S2D_Text *txt, GLfloat angle, int position) {

  S2D_GL_Point p = S2D_GetRectRotationPoint(
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

  if (txt->texture_id == 0) {
    SDL_Color color = { 255, 255, 255 };
    txt->surface = TTF_RenderText_Blended(txt->font_data, txt->msg, color);
    if (!txt->surface) {
      S2D_Error("TTF_RenderText_Blended", TTF_GetError());
      return;
    }
    S2D_GL_CreateTexture(&txt->texture_id, GL_RGBA,
                         txt->width, txt->height,
                         txt->surface->pixels, GL_NEAREST);
    SDL_FreeSurface(txt->surface);
  }

  S2D_GL_DrawText(txt);
}


/*
 * Free the text
 */
void S2D_FreeText(S2D_Text *txt) {
  if (!txt) return;
  free(txt->msg);
  S2D_GL_FreeTexture(&txt->texture_id);
  TTF_CloseFont(txt->font_data);
  free(txt);
}
