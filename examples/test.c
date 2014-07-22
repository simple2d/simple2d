// test.c
#include <simple2d.h>

Window *window;
Image *img_jpg;
Image *img_png;

void on_key(const char *key) {
  printf("on_key: %s\n", key);
}

void key_down(const char *key) {
  printf("key_down: %s\n", key);
}

void update() {
  S2D_DrawTriangle(10, 10, 1.0, 0, 0, 1.0,
                   100, 100, 0, 1.0, 0, 1.0,
                   window->cursor_x, window->cursor_y, 0, 0, 1.0, 1.0);
  
  S2D_DrawQuad(window->cursor_x, window->cursor_y, 1.0, 0, 0, 0.5,
               370, 70, 1.0, 0, 0, 0.5,
               350, 250, 1.0, 0, 0, 0.5,
               70, 270, 1.0, 0, 0, 0.5);
  
  S2D_DrawImage(img_jpg, window->cursor_x, window->cursor_y);
  S2D_DrawImage(img_png, 300, 300);
  
  puts("--------------------");
  printf("cursor: %i, %i\n", window->cursor_x, window->cursor_y);
  printf("frames: %i\n", window->frames);
  printf("total_ms: %i\n", window->total_ms);
  printf("loop_ms: %i\n", window->loop_ms);
  printf("delay_ms: %i\n", window->delay_ms);
  printf("fps: %f\n", window->fps);
}

int main(int argc, char const *argv[]) {
  window = S2D_CreateWindow("hello", 800, 600, 60, true, update, on_key, key_down);
  
  img_jpg = S2D_CreateImage(window, "media/tile.jpg");
  img_png = S2D_CreateImage(window, "media/tile.png");
  
  S2D_Show(window);
  return 0;
}
