// testcard.c
#include <simple2d.h>

Window *window;
Image *img_jpg;
Image *img_png;
Text *text;
Text *on_key_text;
Text *on_key_char;
Text *key_down_text;
Text *key_down_char;
char *font = "./media/bitstream_vera/vera.ttf";
int font_size = 24;

void on_key(const char *key) {
  S2D_FreeText(on_key_char);
  on_key_char = S2D_CreateText(window, font, (char*)key, font_size);
}

void key_down(const char *key) {
  S2D_FreeText(key_down_char);
  key_down_char = S2D_CreateText(window, font, (char*)key, font_size);
}

void render() {
  
  // Primary colors
  
  S2D_DrawQuad( 0,   0, 1, 0, 0, 1,
               50,   0, 1, 0, 0, 1,
               50, 100, 1, 0, 0, 1,
                0, 100, 1, 0, 0, 1);
  
  S2D_DrawQuad( 50,   0, 0, 1, 0, 1,
               100,   0, 0, 1, 0, 1,
               100, 100, 0, 1, 0, 1,
                50, 100, 0, 1, 0, 1);
  
  S2D_DrawQuad(100,   0, 0, 0, 1, 1,
               150,   0, 0, 0, 1, 1,
               150, 100, 0, 0, 1, 1,
               100, 100, 0, 0, 1, 1);
  
  // Color combinations
  
  S2D_DrawQuad(150,   0, 0, 1, 1, 1,
               200,   0, 0, 1, 1, 1,
               200, 100, 0, 1, 1, 1,
               150, 100, 0, 1, 1, 1);
  
  S2D_DrawQuad(200,   0, 1, 0, 1, 1,
               250,   0, 1, 0, 1, 1,
               250, 100, 1, 0, 1, 1,
               200, 100, 1, 0, 1, 1);
  
  S2D_DrawQuad(250,   0, 1, 1, 0, 1,
               300,   0, 1, 1, 0, 1,
               300, 100, 1, 1, 0, 1,
               250, 100, 1, 1, 0, 1);
  
  S2D_DrawQuad(  0, 100, 1, 1, 1, 1,
               800, 100, 0, 0, 0, 1,
               800, 125, 0, 0, 0, 1,
                 0, 125, 1, 1, 1, 1);
  
  S2D_DrawQuad(  0, 125, 1, 0, 0, 1,
               400, 125, 0, 1, 0, 1,
               400, 175, 0, 1, 0, 1,
                 0, 175, 1, 0, 0, 1);
  
  S2D_DrawQuad(400, 125, 0, 1, 0, 1,
               800, 125, 0, 0, 1, 1,
               800, 175, 0, 0, 1, 1,
               400, 175, 0, 1, 0, 1);
  
  S2D_DrawQuad(0,   165, 1, 1, 1, 0,
               800, 165, 1, 1, 1, 1,
               800, 225, 1, 1, 1, 1,
               0,   225, 1, 1, 1, 0);
  
  S2D_DrawImage(img_jpg, 25,  200);
  S2D_DrawImage(img_png, 675, 200);
  S2D_DrawText(text, 25, 400);
  
  S2D_DrawText(on_key_text, 25, 450);
  S2D_DrawText(on_key_char, 125, 450);
  S2D_DrawText(key_down_text, 25, 500);
  S2D_DrawText(key_down_char, 160, 500);
  
  // Follow cursor
  S2D_DrawQuad(window->cursor_x, window->cursor_y, 1, 1, 1, 1,
               window->cursor_x + 10, window->cursor_y, 1, 1, 1, 1,
               window->cursor_x + 10, window->cursor_y + 10, 1, 1, 1, 1,
               window->cursor_x, window->cursor_y + 10, 1, 1, 1, 1);
  
  // if (window->frames % 100 == 0) {
    puts("--------------------");
    printf("cursor: %i, %i\n", window->cursor_x, window->cursor_y);
    printf("frames: %i\n", window->frames);
    printf("total_ms: %i\n", window->total_ms);
    printf("loop_ms: %i\n", window->loop_ms);
    printf("delay_ms: %i\n", window->delay_ms);
    printf("fps: %f\n", window->fps);
  // }
}

int main(int argc, char const *argv[]) {
  window = S2D_CreateWindow("hello", 800, 600, 60, true, NULL, render, on_key, key_down);
  
  img_jpg = S2D_CreateImage(window, "media/tile.jpg");
  img_png = S2D_CreateImage(window, "media/tile.png");
  text = S2D_CreateText(window, font, "Hello World", font_size);
  
  on_key_text = S2D_CreateText(window, font, "On Key:", font_size);
  on_key_char = S2D_CreateText(window, font, "", font_size);
  key_down_text = S2D_CreateText(window, font, "Key Down:", font_size);
  key_down_char = S2D_CreateText(window, font, "", font_size);
  
  S2D_Show(window);
  return 0;
}
