// shapes.c

#include "../include/simple2d.h"


/*
 * Draw a triangle
 */
void S2D_DrawTriangle(GLfloat x1,  GLfloat y1,
                      GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                      GLfloat x2,  GLfloat y2,
                      GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                      GLfloat x3,  GLfloat y3,
                      GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  S2D_GL_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                      x2, y2, c2r, c2g, c2b, c2a,
                      x3, y3, c3r, c3g, c3b, c3a);
}


/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(GLfloat x1,  GLfloat y1,
                  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                  GLfloat x2,  GLfloat y2,
                  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                  GLfloat x3,  GLfloat y3,
                  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a,
                  GLfloat x4,  GLfloat y4,
                  GLfloat c4r, GLfloat c4g, GLfloat c4b, GLfloat c4a) {
  
  S2D_GL_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                      x2, y2, c2r, c2g, c2b, c2a,
                      x3, y3, c3r, c3g, c3b, c3a);
  
  S2D_GL_DrawTriangle(x3, y3, c3r, c3g, c3b, c3a,
                      x4, y4, c4r, c4g, c4b, c4a,
                      x1, y1, c1r, c1g, c1b, c1a);
};


/*
 * Draw a line from a quad
 */
void S2D_DrawLine(GLfloat x1,  GLfloat y1,  GLfloat x2,  GLfloat y2,
                  GLfloat width,
                  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a,
                  GLfloat c4r, GLfloat c4g, GLfloat c4b, GLfloat c4a) {
  
  double length = sqrt(powf(x1 - x2, 2) + powf(y1 - y2, 2));
  double x = ((x2 - x1) / length) * width / 2;
  double y = ((y2 - y1) / length) * width / 2;
  
  S2D_DrawQuad(
    x1 - y, y1 + x, c1r, c1g, c1b, c1a,
    x1 + y, y1 - x, c2r, c2g, c2b, c2a,
    x2 + y, y2 - x, c3r, c3g, c3b, c3a,
    x2 - y, y2 + x, c4r, c4g, c4b, c4a
  );
};
