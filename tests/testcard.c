// testcard.c
#include <simple2d.h>

Window *window;
Image img_bmp;
Image img_jpg;
Image img_png;
Text txt;
Text on_key_text;
Text on_key_char;
Text key_down_text;
Text key_down_char;
Text fps;
Text fps_val;
char fps_str[7];
char *font = "./media/bitstream_vera/vera.ttf";
int font_size = 24;

void update_new() {
  puts("Update callback changed.");
}
void render_new() {
  puts("Render callback changed.");
}

void on_key(const char *key) {
  printf("Key pressed: %s\n", key);
  
  // Update text with key pressed
  S2D_SetText(&on_key_char, (char*)key);
  
  if (*key == 'C') {
    window->update = update_new;
    window->render = render_new;
  }
}

void on_key_down(const char *key) {
  // Update text with key held down
  S2D_SetText(&key_down_char, (char*)key);
}

void update() {
  // printf("elapsed_ms: %i\n", window->elapsed_ms);
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
  
  // Images
  img_bmp.x = 25;
  img_bmp.y = 200;
  S2D_DrawImage(img_bmp);
  
  img_jpg.x = 150;
  img_jpg.y = 200;
  S2D_DrawImage(img_jpg);
  
  img_png.x = 675;
  img_png.y = 200;
  S2D_DrawImage(img_png);
  
  // Text
  
  txt.x = 25;
  txt.y = 400;
  txt.color.r = window->cursor.x / 400.0;
  txt.color.g = window->cursor.y / 400.0;
  txt.w = window->cursor.x;
  txt.h = window->cursor.y;
  S2D_DrawText(txt);
  
  // Input
  
  on_key_text.x = 25;
  on_key_text.y = 450;
  S2D_DrawText(on_key_text);
  
  on_key_char.x = 125;
  on_key_char.y = 450;
  S2D_DrawText(on_key_char);
  
  key_down_text.x = 25;
  key_down_text.y = 500;
  S2D_DrawText(key_down_text);
  
  key_down_char.x = 160;
  key_down_char.y = 500;
  S2D_DrawText(key_down_char);
  
  // Crosshairs
  
  S2D_DrawQuad(0, window->cursor.y, 1, 1, 1, 1,
               window->width, window->cursor.y, 1, 1, 1, 1,
               window->width, window->cursor.y + 1, 1, 1, 1, 1,
               0, window->cursor.y + 1, 1, 1, 1, 1);
  
  S2D_DrawQuad(window->cursor.x, 0, 1, 1, 1, 1,
               window->cursor.x, window->height, 1, 1, 1, 1,
               window->cursor.x + 1, window->width, 1, 1, 1, 1,
               window->cursor.x + 1, 0, 1, 1, 1, 1);
  
  // Draw window stats
  
  S2D_DrawText(fps);
  snprintf(fps_str, 7, "%f", window->fps);
  S2D_SetText(&fps_val, fps_str);
  S2D_DrawText(fps_val);
  
  // TODO: Make option to print this
  puts("--------------------");
  printf("cursor: %i, %i\n", window->cursor.x, window->cursor.y);
  printf("frames: %i\n", window->frames);
  printf("elapsed_ms: %i\n", window->elapsed_ms);
  printf("loop_ms: %i\n", window->loop_ms);
  printf("delay_ms: %i\n", window->delay_ms);
  printf("fps: %f\n", window->fps);
}

int main(int argc, char const *argv[]) {
  window = S2D_CreateWindow("Testcard", 800, 600, update, render);
  
  window->on_key = on_key;
  window->on_key_down = on_key_down;
  window->background.r = 0.2;
  window->background.g = 0.2;
  window->background.b = 0.2;
  // window->fps_cap = 20;
  
  img_bmp = S2D_CreateImage("media/image.bmp");
  img_jpg = S2D_CreateImage("media/image.jpg");
  img_png = S2D_CreateImage("media/image.png");
  txt = S2D_CreateText(font, "Hello World", font_size);
  
  on_key_text = S2D_CreateText(font, "On Key:", font_size);
  on_key_char = S2D_CreateText(font, " ", font_size);
  key_down_text = S2D_CreateText(font, "Key Down:", font_size);
  key_down_char = S2D_CreateText(font, " ", font_size);
  
  fps = S2D_CreateText(font, "FPS:", font_size);
  fps.x = 630;
  fps.y = 15;
  
  fps_val = S2D_CreateText(font, " ", font_size);
  fps_val.x = 690;
  fps_val.y = 15;
  
  S2D_Show(window);
  return 0;
}
