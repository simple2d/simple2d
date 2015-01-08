// OpenGL 2.1

#include "../include/simple2d.h"


/*
 * Testing
 */
void hello_gl2() {
  puts("Hello from OpenGL 2.1!");
}


/*
 * Initalize OpenGL
 */
int init_gl2(int width, int height) {
  
  print_gl_context();
  
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
void draw_triangle_gl2(GLfloat x1,  GLfloat y1,
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
 * Draw image
 */
void draw_image_gl2(Image img) {
  
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
}


/*
 * Draw text
 */
void draw_text_gl2(Text txt) {
  
  int w, h, w_v, h_v;
  
  TTF_SizeText(txt.font, txt.msg, &w, &h);
  
  if (SDL_GL_BindTexture(txt.texture, NULL, NULL)) {
    // This is currently returning `-1`, but still works somehow
    // Uncomment this when returns `0`:
    //   sdl_error("draw_text_gl -> SDL_GL_BindTexture");
  }
  
  // If text width or height is 0, use calculated values
  if (!txt.w || !txt.h) {
    w_v = w;
    h_v = h;
  } else {
    w_v = txt.w;
    h_v = txt.h;
  }
  
  glBegin(GL_QUADS);
    glColor4f(txt.color.r, txt.color.g, txt.color.b, txt.color.a);
    glTexCoord2f(0, 0); glVertex2f(txt.x, txt.y);
    glTexCoord2f(w, 0); glVertex2f(txt.x + w_v, txt.y);
    glTexCoord2f(w, h); glVertex2f(txt.x + w_v, txt.y + h_v);
    glTexCoord2f(0, h); glVertex2f(txt.x, txt.y + h_v);
  glEnd();
  
  SDL_GL_UnbindTexture(txt.texture);
}
