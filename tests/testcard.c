// testcard.c
#include <simple2d.h>

S2D_Window *window;
S2D_Image  *img_bmp;
S2D_Image  *img_jpg;
S2D_Image  *img_png;
S2D_Text   *on_key_text;
S2D_Text   *on_key_char;
S2D_Text   *key_down_text;
S2D_Text   *key_down_char;
S2D_Text   *fps;
S2D_Text   *fps_val;

char fps_str[7];
const char *font = "media/bitstream_vera/vera.ttf";
int font_size = 20;

typedef struct {
  int x;
  int y;
} Point;

Point pointer;


void on_key(const char *key) {
  printf("Key pressed: %s\n", key);
  
  if (strcmp(key, "Escape") == 0) {
    S2D_Close(window);
  }
  
  S2D_SetText(on_key_char, key);
}

void on_key_down(const char *key) {
  S2D_SetText(key_down_char, key);
}

void on_mouse(int x, int y) {
  printf("Mouse down at: %i, %i\n", x, y);
}

void on_controller(bool is_axis, int axis, int val, bool is_btn, int btn) {
}

void update() {
  pointer.x = window->mouse.x;
  pointer.y = window->mouse.y;
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
  
  S2D_DrawQuad(150,  0, 0, 1, 1, 1,
               200,  0, 0, 1, 1, 1,
               200, 50, 0, 1, 1, 1,
               150, 50, 0, 1, 1, 1);
  
  S2D_DrawQuad(200,  0, 1, 0, 1, 1,
               250,  0, 1, 0, 1, 1,
               250, 50, 1, 0, 1, 1,
               200, 50, 1, 0, 1, 1);
  
  S2D_DrawQuad(250,  0, 1, 1, 0, 1,
               325,  0, 1, 1, 0, 1,
               325, 50, 1, 1, 0, 1,
               250, 50, 1, 1, 0, 1);
  
  S2D_DrawQuad(150, 50,  1, .5, 0, 1,
               200, 50,  1, .5, 0, 1,
               200, 100, 1, .5, 0, 1,
               150, 100, 1, .5, 0, 1);
  
  S2D_DrawQuad(200,  50, .5, 1, 0, 1,
               250,  50, .5, 1, 0, 1,
               250, 100, .5, 1, 0, 1,
               200, 100, .5, 1, 0, 1);
  
  S2D_DrawQuad(250,  50, 1, 0, .5, 1,
               325,  50, 1, 0, .5, 1,
               325, 100, 1, 0, .5, 1,
               250, 100, 1, 0, .5, 1);
  
  // White to black gradient
  
  S2D_DrawQuad(  0, 100, 1, 1, 1, 1,
               600, 100, 0, 0, 0, 1,
               600, 125, 0, 0, 0, 1,
                 0, 125, 1, 1, 1, 1);
  
  // Color gradient
  
  S2D_DrawQuad(  0, 125, 1, 0, 0, 1,
               600, 125, 0, 1, 0, 1,
               600, 175, 0, 0, 1, 1,
                 0, 175, 1, 1, 0, 1);
  
  // Transparancy
  
  S2D_DrawQuad(  0, 165, 1, 1, 1, 0,
               600, 165, 1, 1, 1, 1,
               600, 200, 1, 1, 1, 1,
                 0, 200, 1, 1, 1, 0);
  
  // Triangles
  
  S2D_DrawTriangle(25, 200, 1, 0, 0, 1,
                   50, 250, 1, 0, 0, 1,
                   0,  250, 1, 0, 0, 1);
  
  S2D_DrawTriangle(75,  200, 0, 1, 0, 1,
                   100, 250, 0, 1, 0, 1,
                   50,  250, 0, 1, 0, 1);
  
  S2D_DrawTriangle(125, 200, 0, 0, 1, 1,
                   150, 250, 0, 0, 1, 1,
                   100, 250, 0, 0, 1, 1);
  
  S2D_DrawTriangle(175, 200, 1, 0, 0, 1,
                   200, 250, 0, 1, 0, 1,
                   150, 250, 0, 0, 1, 1);
  
  S2D_DrawTriangle(175, 200, 1, 0, 0, 1,
                   200, 250, 0, 1, 0, 1,
                   150, 250, 0, 0, 1, 1);
  
  S2D_DrawQuad(200, 200, 1, 1, 1, .5,
               250, 200, 1, 1, 1, .5,
               250, 250, 1, 1, 1, .5,
               200, 250, 1, 1, 1, .5);
  
  S2D_DrawTriangle(225, 200, 1, 1, 1, 1,
                   250, 250, 0, 0, 0, 1,
                   200, 250, 1, 1, 1, 0);
  
  // Quadrilaterals
  
  S2D_DrawQuad(300, 200, 1, 0, 0, 1,
               350, 200, 0, 1, 0, 1,
               300, 250, 0, 0, 1, 1,
               250, 250, 1, 1, 0, 1);
  
  S2D_DrawQuad(250, 200, 1, 1, 1, 0,
               300, 200, 1, 1, 1, 0,
               350, 250, 1, 1, 1, 1,
               300, 250, 1, 1, 1, 0);
  
  // Images
  
  img_png->x = 300;
  img_png->y = 0;
  S2D_DrawImage(img_png);
  
  img_jpg->x = 400;
  img_jpg->y = 0;
  S2D_DrawImage(img_jpg);
  
  img_bmp->x = 500;
  img_bmp->y = 0;
  S2D_DrawImage(img_bmp);
  
  // Text
  
  S2D_DrawText(on_key_text);
  S2D_DrawText(on_key_char);
  S2D_DrawText(key_down_text);
  S2D_DrawText(key_down_char);
  
  // Window stats
  
  S2D_DrawText(fps);
  snprintf(fps_str, 7, "%f", window->fps);
  S2D_SetText(fps_val, fps_str);
  S2D_DrawText(fps_val);
  
  // Mouse positions
  
  S2D_DrawQuad(pointer.x - 5, pointer.y - 7,  1, 1, 1, 1,
               pointer.x + 5, pointer.y - 7,  1, 1, 1, 1,
               pointer.x + 5, pointer.y + 4,  1, 1, 1, 1,
               pointer.x - 5, pointer.y + 4,  1, 1, 1, 1);
}


int main(int argc, const char *argv[]) {
  
  S2D_Diagnostics(true);
  
  window = S2D_CreateWindow("Simple 2D – Testcard", 600, 500, update, render, S2D_RESIZABLE);
  if (!window) return 1;
  
  window->on_key        = on_key;
  window->on_key_down   = on_key_down;
  window->on_mouse      = on_mouse;
  window->on_controller = on_controller;
  
  img_bmp = S2D_CreateImage("media/image.bmp");
  img_jpg = S2D_CreateImage("media/image.jpg");
  img_png = S2D_CreateImage("media/image.png");
  
  on_key_text = S2D_CreateText(font, "On Key:", font_size);
  on_key_char = S2D_CreateText(font, "", font_size);
  on_key_text->x = 5;
  on_key_text->y = 270;
  on_key_char->x = 90;
  on_key_char->y = 270;
  
  key_down_text = S2D_CreateText(font, "On Key Down:", font_size);
  key_down_char = S2D_CreateText(font, "", font_size);
  key_down_text->x = 5;
  key_down_text->y = 300;
  key_down_char->x = 154;
  key_down_char->y = 300;
  
  fps = S2D_CreateText(font, "FPS:", font_size);
  fps->x = 460;
  fps->y = 470;
  
  fps_val = S2D_CreateText(font, "", font_size);
  fps_val->x = 515;
  fps_val->y = 470;
  
  S2D_Show(window);
  
  S2D_FreeWindow(window);
  
  return 0;
}
