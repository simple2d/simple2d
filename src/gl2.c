// OpenGL 2.1

#include "../include/simple2d.h"


/*
 * Applies the projection matrix
 */
void S2D_GL2_ApplyProjection(int w, int h) {

  // Initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Multiply the current matrix with the orthographic matrix
  glOrtho(0.f, w, h, 0.f, -1.f, 1.f);

  // Initialize the model-view matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


/*
 * Initalize OpenGL
 */
int S2D_GL2_Init() {

  GLenum error = GL_NO_ERROR;

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Check for errors
  error = glGetError();
  if (error != GL_NO_ERROR) {
    S2D_GL_PrintError("OpenGL initialization failed");
    return 1;
  } else {
    return 0;
  }
}


/*
 * Draw triangle
 */
void S2D_GL2_DrawTriangle(GLfloat x1,  GLfloat y1,
                          GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                          GLfloat x2,  GLfloat y2,
                          GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                          GLfloat x3,  GLfloat y3,
                          GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {

  glBegin(GL_TRIANGLES);
    glColor4f(c1r, c1g, c1b, c1a); glVertex2f(x1, y1);
    glColor4f(c2r, c2g, c2b, c2a); glVertex2f(x2, y2);
    glColor4f(c3r, c3g, c3b, c3a); glVertex2f(x3, y3);
  glEnd();
}


/*
 * Draw texture
 */
static void S2D_GL2_DrawTexture(int x, int y, int w, int h,
                                GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                                GLfloat tx1, GLfloat ty1, GLfloat tx2, GLfloat ty2,
                                GLfloat tx3, GLfloat ty3, GLfloat tx4, GLfloat ty4,
                                GLuint texture_id) {

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texture_id);

  glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glTexCoord2f(tx1, ty1); glVertex2f(x,     y    );
    glTexCoord2f(tx2, ty2); glVertex2f(x + w, y    );
    glTexCoord2f(tx3, ty3); glVertex2f(x + w, y + h);
    glTexCoord2f(tx4, ty4); glVertex2f(x,     y + h);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}


/*
 * Draw image
 */
void S2D_GL2_DrawImage(S2D_Image *img) {
  S2D_GL2_DrawTexture(
    img->x, img->y, img->width, img->height,
    img->color.r, img->color.g, img->color.b, img->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    img->texture_id
  );
}


/*
 * Draw sprite
 */
void S2D_GL2_DrawSprite(S2D_Sprite *spr) {
  S2D_GL2_DrawTexture(
    spr->x, spr->y, spr->width, spr->height,
    spr->img->color.r, spr->img->color.g, spr->img->color.b, spr->img->color.a,
    spr->tx1, spr->ty1, spr->tx2, spr->ty2, spr->tx3, spr->ty3, spr->tx4, spr->ty4,
    spr->img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GL2_DrawText(S2D_Text *txt) {
  S2D_GL2_DrawTexture(
    txt->x, txt->y, txt->width, txt->height,
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    txt->texture_id
  );
}
