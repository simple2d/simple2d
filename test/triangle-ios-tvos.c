#include <Simple2D/simple2d.h>

S2D_Window *window;

void render() {
  S2D_DrawQuad(
    0, 0, 1, 0, 1, 1,
    window->width, 0, 0, 1, 0, 1,
    window->width, window->height, 0, 0, 1, 1,
    0, window->height, 0, 1, 1, 1
  );
  S2D_DrawTriangle(
    window->width / 2, window->height / 5, 1, 0, 0, 1,
    window->width, window->height / 1.5, 0, 1, 0, 1,
    0, window->height / 1.5, 0, 0, 1, 1
  );
}

int main() {

  window = S2D_CreateWindow(
    "Hello Triangle", S2D_DISPLAY_WIDTH, S2D_DISPLAY_HEIGHT, NULL, render, 0
  );

  S2D_Show(window);
  S2D_FreeWindow(window);
  return 0;
}
