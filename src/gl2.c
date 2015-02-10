// OpenGL 2.1

#include "../include/simple2d.h"


/*
 * Initalize OpenGL
 */
int gl2_init(int width, int height) {
  
  gl_print_context();
  
  GLenum error = GL_NO_ERROR;
  
  glViewport(0, 0, width, height);
  
  // Initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Multiply the current matrix with the orthographic matrix
  glOrtho(0.f, width, height, 0.f, -1.f, 1.f);
  
  // Initialize the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Check for errors
  error = glGetError();
  if (error != GL_NO_ERROR) {
    printf( "OpenGL initialization failed! Error code: %i\n", error);
    return 1;
  } else {
    return 0;
  }
}


/*
 * Draw triangle
 */
void gl2_draw_triangle(GLfloat x1,  GLfloat y1,
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
static void gl2_draw_texture(int x, int y, int w, int h, 
                             GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                             GLuint texture_id) {
  
  glEnable(GL_TEXTURE_2D);
  
  glBindTexture(GL_TEXTURE_2D, texture_id);
  
  glBegin(GL_QUADS);
    glColor4f(r, g, b, a);
    glTexCoord2f(0, 0); glVertex2f(x,     y    );
    glTexCoord2f(1, 0); glVertex2f(x + w, y    );
    glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
    glTexCoord2f(0, 1); glVertex2f(x,     y + h);
  glEnd();
  
  glDisable(GL_TEXTURE_2D);
}


/*
 * Draw image
 */
void gl2_draw_image(Image img) {
  gl2_draw_texture(
    img.x, img.y, img.w, img.h,
    1.f, 1.f, 1.f, 1.f,
    img.texture_id
  );
}


/*
 * Draw text
 */
void gl2_draw_text(Text txt) {
  gl2_draw_texture(
    txt.x, txt.y, txt.w, txt.h,
    txt.color.r, txt.color.g, txt.color.b, txt.color.a,
    txt.texture_id
  );
}
