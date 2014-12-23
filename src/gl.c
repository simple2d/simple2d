// OpenGL

#include "../include/simple2d.h"


/*
 * Testing
 */
void hello_gl() {
  puts("Hello from OpenGL!");
}


/*
 * Initalize OpenGL
 */
int init_gl(int width, int height) {
  
  printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
  printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
  printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
  printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  
  GLenum error = GL_NO_ERROR;
  
  glViewport(0, 0, width, height);
  glOrtho(0, width, height, 0, -1.f, 1.f);
  
  // Initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
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
void draw_triangle_gl(GLfloat x1,  GLfloat y1,
                      GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                      GLfloat x2,  GLfloat y2,
                      GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                      GLfloat x3,  GLfloat y3,
                      GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glBegin(GL_TRIANGLES);
    glColor4f(c1r, c1g, c1b, c1a); glVertex2f(x1, y1);
    glColor4f(c2r, c2g, c2b, c2a); glVertex2f(x2, y2);
    glColor4f(c3r, c3g, c3b, c3a); glVertex2f(x3, y3);
  glEnd();
}


/*
 * Draw image
 */
void draw_image_gl(Image img) {
  
  int w, h;
  SDL_QueryTexture(img.texture, NULL, NULL, &w, &h);
  
  if (SDL_GL_BindTexture(img.texture, NULL, NULL)) {
    sdl_error("draw_image_gl -> SDL_GL_BindTexture");
  }
  
  glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0); glVertex2f(img.x, img.y);
    glTexCoord2f(w, 0); glVertex2f(img.x + w, img.y);
    glTexCoord2f(w, h); glVertex2f(img.x + w, img.y + h);
    glTexCoord2f(0, h); glVertex2f(img.x, img.y + h);
  glEnd();
  
  SDL_GL_UnbindTexture(img.texture);
}


/*
 * Draw text
 */
void draw_text_gl(Text txt) {
  
  int w, h;
  TTF_SizeText(text->font, text->msg, &w, &h);
  
  
  
  glBegin(GL_QUADS);
    glColor4f(1, 1, 1, 1);
    glTexCoord2f(0, 0); glVertex2f(x, y);
    glTexCoord2f(w, 0); glVertex2f(x + w, y);
    glTexCoord2f(w, h); glVertex2f(x + w, y + h);
    glTexCoord2f(0, h); glVertex2f(x, y + h);
  glEnd();
  
  SDL_GL_UnbindTexture(txt.texture);
}
