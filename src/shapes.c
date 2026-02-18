// shapes.c

#include "../include/simple2d.h"


/*
 * Rotate a point around a given point
 * Params:
 *   p      The point to rotate
 *   angle  The angle in degrees
 *   rx     The x coordinate to rotate around
 *   ry     The y coordinate to rotate around
 */
S2D_Point S2D_RotatePoint(S2D_Point p, float angle, float rx, float ry) {

  // Convert from degrees to radians
  angle = angle * M_PI / 180.0;

  // Get the sine and cosine of the angle
  float sa = sinf(angle);
  float ca = cosf(angle);

  // Translate point to origin
  p.x -= rx;
  p.y -= ry;

  // Rotate point
  float xnew = p.x * ca - p.y * sa;
  float ynew = p.x * sa + p.y * ca;

  // Translate point back
  p.x = xnew + rx;
  p.y = ynew + ry;

  return p;
}


/*
 * Get the point to be rotated around given a position in a rectangle
 */
S2D_Point S2D_GetRectRotationPoint(int x, int y, int w, int h, int position) {

  S2D_Point p;

  switch (position) {
    case S2D_CENTER:
      p.x = x + (w / 2.0);
      p.y = y + (h / 2.0);
      break;
    case S2D_TOP_LEFT:
      p.x = x;
      p.y = y;
      break;
    case S2D_TOP_RIGHT:
      p.x = x + w;
      p.y = y;
      break;
    case S2D_BOTTOM_LEFT:
      p.x = x;
      p.y = y + h;
      break;
    case S2D_BOTTOM_RIGHT:
      p.x = x + w;
      p.y = y + h;
      break;
    default:
      p.x = x + (w / 2.0);
      p.y = y + (h / 2.0);
      break;
  }

  return p;
}


/*
 * Draws a filled triangle with per-vertex colors.
 *
 * Parameters:
 *   x, y - Vertex position coordinates
 *   r, g, b, a - Vertex color values (red, green, blue, alpha)
 *   The numbers specify the coordinates and color for each vertex of the triangle
 *
 * The triangle is rendered using the current SDL renderer, and colors are interpolated between vertices.
 */
void S2D_DrawTriangle(float x1, float y1,
                      float r1, float g1, float b1, float a1,
                      float x2, float y2,
                      float r2, float g2, float b2, float a2,
                      float x3, float y3,
                      float r3, float g3, float b3, float a3) {

  SDL_Vertex vertices[3] = {{
      .position  = { x1, y1 },
      .color     = { r1, g1, b1, a1 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x2, y2 },
      .color     = { r2, g2, b2, a2 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x3, y3 },
      .color     = { r3, g3, b3, a3 },
      .tex_coord = { 0.0f, 0.0f }
  }};

  // Update coordinates for high DPI window
  S2D_WindowToRendererCoordinates(vertices, 3);

  SDL_RenderGeometry(s2d_app.window->sdl_renderer, NULL, vertices, 3, NULL, 0);
}


/*
 * Draws a filled quad with per-vertex colors.
 *
 * Parameters:
 *   x, y - Vertex position coordinates for each of the 4 corners
 *   r, g, b, a - Vertex color values (red, green, blue, alpha) for each corner
 *
 * The quad is rendered as two triangles with interpolated colors.
 */
void S2D_DrawQuad(float x1, float y1,
                  float r1, float g1, float b1, float a1,
                  float x2, float y2,
                  float r2, float g2, float b2, float a2,
                  float x3, float y3,
                  float r3, float g3, float b3, float a3,
                  float x4, float y4,
                  float r4, float g4, float b4, float a4) {

  SDL_Vertex vertices[4] = {{
      .position  = { x1, y1 },
      .color     = { r1, g1, b1, a1 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x2, y2 },
      .color     = { r2, g2, b2, a2 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x3, y3 },
      .color     = { r3, g3, b3, a3 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x4, y4 },
      .color     = { r4, g4, b4, a4 },
      .tex_coord = { 0.0f, 0.0f }
  }};

  // Indices for two triangles forming the quad
  int indices[6] = { 0, 1, 2, 0, 2, 3 };

  // Update coordinates for high DPI window
  S2D_WindowToRendererCoordinates(vertices, 4);

  SDL_RenderGeometry(s2d_app.window->sdl_renderer, NULL, vertices, 4, indices, 6);
}


/*
 * Draws a line with specified width and per-vertex colors.
 *
 * Parameters:
 *   x1, y1 - Start point of the line
 *   x2, y2 - End point of the line
 *   width - Thickness of the line
 *   r, g, b, a - Color values for each of the 4 corners of the line quad
 *
 * The line is rendered as a quad perpendicular to the line direction.
 */
void S2D_DrawLine(float x1, float y1, float x2, float y2,
                  float width,
                  float r1, float g1, float b1, float a1,
                  float r2, float g2, float b2, float a2,
                  float r3, float g3, float b3, float a3,
                  float r4, float g4, float b4, float a4) {

  // Calculate the direction vector of the line
  float dx = x2 - x1;
  float dy = y2 - y1;

  // Calculate the length of the line
  float length = sqrtf(dx * dx + dy * dy);

  // Avoid division by zero for zero-length lines
  if (length == 0.0f) return;

  // Calculate the perpendicular unit vector scaled by half width
  float half_width = width / 2.0f;
  float px = (-dy / length) * half_width;
  float py = (dx / length) * half_width;

  // Calculate the four corners of the line quad
  SDL_Vertex vertices[4] = {{
      .position  = { x1 + px, y1 + py },
      .color     = { r1, g1, b1, a1 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x1 - px, y1 - py },
      .color     = { r2, g2, b2, a2 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x2 - px, y2 - py },
      .color     = { r3, g3, b3, a3 },
      .tex_coord = { 0.0f, 0.0f }
    }, {
      .position  = { x2 + px, y2 + py },
      .color     = { r4, g4, b4, a4 },
      .tex_coord = { 0.0f, 0.0f }
  }};

  // Indices for two triangles forming the quad
  int indices[6] = { 0, 1, 2, 0, 2, 3 };

  // Update coordinates for high DPI window
  S2D_WindowToRendererCoordinates(vertices, 4);

  SDL_RenderGeometry(s2d_app.window->sdl_renderer, NULL, vertices, 4, indices, 6);
}


/*
 * Draws a filled circle with a solid color.
 *
 * Parameters:
 *   x, y - Center position of the circle
 *   radius - Radius of the circle
 *   sectors - Number of triangular sectors (higher = smoother circle)
 *   r, g, b, a - Color values for the circle
 *
 * The circle is rendered as a triangle fan from the center.
 */
void S2D_DrawCircle(float x, float y, float radius, int sectors,
                    float r, float g, float b, float a) {

  // Need at least 3 sectors to form a shape
  if (sectors < 3) sectors = 3;

  // Total vertices: 1 center + (sectors + 1) edge points
  int num_vertices = sectors + 2;
  SDL_Vertex *vertices = (SDL_Vertex *)malloc(num_vertices * sizeof(SDL_Vertex));

  // Center vertex
  vertices[0].position.x = x;
  vertices[0].position.y = y;
  vertices[0].color.r = r;
  vertices[0].color.g = g;
  vertices[0].color.b = b;
  vertices[0].color.a = a;
  vertices[0].tex_coord.x = 0.0f;
  vertices[0].tex_coord.y = 0.0f;

  // Edge vertices
  float angle_step = (2.0f * M_PI) / sectors;
  for (int i = 0; i <= sectors; i++) {
    float angle = i * angle_step;
    vertices[i + 1].position.x = x + radius * cosf(angle);
    vertices[i + 1].position.y = y + radius * sinf(angle);
    vertices[i + 1].color.r = r;
    vertices[i + 1].color.g = g;
    vertices[i + 1].color.b = b;
    vertices[i + 1].color.a = a;
    vertices[i + 1].tex_coord.x = 0.0f;
    vertices[i + 1].tex_coord.y = 0.0f;
  }

  // Create indices for triangle fan
  int num_indices = sectors * 3;
  int *indices = (int *)malloc(num_indices * sizeof(int));

  for (int i = 0; i < sectors; i++) {
    indices[i * 3 + 0] = 0;         // Center
    indices[i * 3 + 1] = i + 1;     // Current edge vertex
    indices[i * 3 + 2] = i + 2;     // Next edge vertex
  }

  // Update coordinates for high DPI window
  S2D_WindowToRendererCoordinates(vertices, num_vertices);

  SDL_RenderGeometry(s2d_app.window->sdl_renderer, NULL, vertices, num_vertices, indices, num_indices);

  free(vertices);
  free(indices);
}
