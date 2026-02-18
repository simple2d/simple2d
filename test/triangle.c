#include <simple2d.h>

void S2D_Init() {
  S2D_CreateWindow("Hello Triangle", 640, 480);
}

void S2D_Render() {
  S2D_DrawTriangle(
    320,  50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  );
}
