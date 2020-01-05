#include <simple2d.h>

static float orthoMatrix[4] = {1.0, 2.0, 3.0, 4.0};

void render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}

int main() {

  objc_say_hello();

  int a = objc_plus(1, 2);
  printf("plus: %i\n", a);

  objc_matrix(orthoMatrix);

  S2D_Window *window = S2D_CreateWindow(
    "Hello Triangle", 640, 480, NULL, render, 0
  );

  S2D_Show(window);
  return 0;
}
