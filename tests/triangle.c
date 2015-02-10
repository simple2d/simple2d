#include <simple2d.h>

Window *window;

void render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}

int main(int argc, char const *argv[]) {
  
  window = S2D_CreateWindow(
    "Hello Triangle", 640, 480, NULL, render
  );
  
  S2D_Show(window);
  return 0;
}
