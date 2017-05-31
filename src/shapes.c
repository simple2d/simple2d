// shapes.c

#include "../include/simple2d.h"


/*
 * Draw a triangle
 */
void S2D_DrawTriangle(GLfloat x1, GLfloat y1,
                      GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                      GLfloat x2, GLfloat y2,
                      GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                      GLfloat x3, GLfloat y3,
                      GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  S2D_GL_DrawTriangle(x1, y1, r1, g1, b1, a1,
                      x2, y2, r2, g2, b2, a2,
                      x3, y3, r3, g3, b3, a3);
}


/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(GLfloat x1, GLfloat y1,
                  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                  GLfloat x2, GLfloat y2,
                  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                  GLfloat x3, GLfloat y3,
                  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3,
                  GLfloat x4, GLfloat y4,
                  GLfloat r4, GLfloat g4, GLfloat b4, GLfloat a4) {

  S2D_GL_DrawTriangle(x1, y1, r1, g1, b1, a1,
                      x2, y2, r2, g2, b2, a2,
                      x3, y3, r3, g3, b3, a3);

  S2D_GL_DrawTriangle(x3, y3, r3, g3, b3, a3,
                      x4, y4, r4, g4, b4, a4,
                      x1, y1, r1, g1, b1, a1);
};


/*
 * Draw a line from a quad
 */
void S2D_DrawLine(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2,
                  GLfloat width,
                  GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                  GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                  GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3,
                  GLfloat r4, GLfloat g4, GLfloat b4, GLfloat a4) {

  double length = sqrt(powf(x1 - x2, 2) + powf(y1 - y2, 2));
  double x = ((x2 - x1) / length) * width / 2;
  double y = ((y2 - y1) / length) * width / 2;

  S2D_DrawQuad(
    x1 - y, y1 + x, r1, g1, b1, a1,
    x1 + y, y1 - x, r2, g2, b2, a2,
    x2 + y, y2 - x, r3, g3, b3, a3,
    x2 - y, y2 + x, r4, g4, b4, a4
  );
};
