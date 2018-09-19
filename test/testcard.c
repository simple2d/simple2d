// testcard.c
#include <simple2d.h>

S2D_Window *window;
S2D_Image  *img_bmp;
S2D_Image  *img_jpg;
S2D_Image  *img_png;
S2D_Image  *img_r;
S2D_Image  *img_g;
S2D_Image  *img_b;
S2D_Sprite *spr;
int sprite_step = 0;
S2D_Text   *txt_r;
S2D_Text   *txt_g;
S2D_Text   *txt_b;
S2D_Text   *fps;
S2D_Text   *fps_val;

const char *font = "media/bitstream_vera/vera.ttf";
int font_size = 20;

typedef struct {
  int x;
  int y;
} Point;

Point pointer;
Point click_pointer;

bool mouse_click = false;
bool rotate = false;


void on_key(S2D_Event e) {
  switch (e.type) {
    case S2D_KEY_DOWN:
      printf("Key down: %s\n", e.key);
      if (strcmp(e.key, "Escape") == 0) S2D_Close(window);
      if (strcmp(e.key, "R") == 0) rotate = rotate ? false : true;
      break;

    case S2D_KEY_HELD:
      printf("Key held: %s\n", e.key);
      break;

    case S2D_KEY_UP:
      printf("Key up: %s\n", e.key);
      break;
  }
}


void print_mouse_button(int e) {
  switch (e) {
    case S2D_MOUSE_LEFT:
      puts("Button left");
      break;
    case S2D_MOUSE_MIDDLE:
      puts("Button middle");
      break;
    case S2D_MOUSE_RIGHT:
      puts("Button right");
      break;
    case S2D_MOUSE_X1:
      puts("Button X1");
      break;
    case S2D_MOUSE_X2:
      puts("Button X2");
      break;
  }
}


void on_mouse(S2D_Event e) {
  puts("=== Mouse Event ===");

  switch (e.type) {
    case S2D_MOUSE_DOWN:
      puts("Mouse down");
      print_mouse_button(e.button);
      if (e.dblclick) puts("Double click");
      mouse_click = true;
      click_pointer.x = e.x;
      click_pointer.y = e.y;
      break;

    case S2D_MOUSE_UP:
      puts("Mouse up");
      print_mouse_button(e.button);
      if (e.dblclick) puts("Double click");
      break;

    case S2D_MOUSE_SCROLL:
      puts("Mouse scroll");
      if (e.direction == S2D_MOUSE_SCROLL_NORMAL) {
        puts("Direction normal");
      } else if (e.direction == S2D_MOUSE_SCROLL_INVERTED) {
        puts("Direction inverted");
      }
      printf("delta x: %i\ndelta y: %i\n", e.delta_x, e.delta_y);
      break;

    case S2D_MOUSE_MOVE:
      puts("Mouse movement");
      printf("delta x: %i\ndelta y: %i\n", e.delta_x, e.delta_y);
      break;
  }

  if (e.type != S2D_MOUSE_SCROLL) printf("x: %i, y: %i\n", e.x, e.y);
}


void on_controller(S2D_Event e) {
  puts("=== Controller Event ===");
  printf("Controller #%i\n", e.which);

  switch (e.type) {
    case S2D_AXIS:
      printf("Axis: %i\n", e.axis);
      printf("Value: %i\n", e.value);
      break;

    case S2D_BUTTON_DOWN:
      printf("Button #%i down\n", e.button);
      break;

    case S2D_BUTTON_UP:
      printf("Button #%i up\n", e.button);
      break;
  }
}


void update() {
  pointer.x = window->mouse.x;
  pointer.y = window->mouse.y;

  if (rotate) {
    img_png->x = window->mouse.x;
    img_png->y = window->mouse.y;

    GLfloat angle = (GLfloat)window->frames;

    S2D_RotateImage(img_png,   angle, S2D_CENTER);
    S2D_RotateImage(img_jpg,   angle, S2D_TOP_LEFT);
    S2D_RotateImage(img_bmp,   angle, S2D_BOTTOM_RIGHT);
    S2D_RotateImage(img_r, angle, S2D_CENTER);
    S2D_RotateImage(img_g, angle, S2D_CENTER);
    S2D_RotateImage(img_b, angle, S2D_CENTER);

    S2D_RotateSprite(spr,      angle, S2D_CENTER);

    S2D_RotateText(txt_r,  angle, S2D_CENTER);
    S2D_RotateText(txt_g,  angle, S2D_CENTER);
    S2D_RotateText(txt_b,  angle, S2D_CENTER);
    S2D_RotateText(fps,        angle, S2D_CENTER);
    S2D_RotateText(fps_val,    angle, S2D_CENTER);
  }
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
                    0, 250, 1, 0, 0, 1);

  S2D_DrawTriangle( 75, 200, 0, 1, 0, 1,
                   100, 250, 0, 1, 0, 1,
                    50, 250, 0, 1, 0, 1);

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

  // Lines

  S2D_DrawLine(
    354, 204, 397, 247,
    11,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1
  );

  S2D_DrawLine(
    395, 205, 355, 245,
    15,
    1, 0, 0, 0.5,
    0, 1, 0, 0.5,
    0, 0, 1, 0.5,
    1, 0, 1, 0.5
  );

  // Circles

  S2D_DrawCircle(525, 225, 25, 20, 1.0, 0.2, 0.2,   1);
  S2D_DrawCircle(575, 225, 25,  8, 0.2, 1.0, 0.2,   1);
  S2D_DrawCircle(575, 225, 17, 16,   0,   0,   0, 0.6);

  // Images

  S2D_DrawImage(img_png);
  S2D_DrawImage(img_jpg);
  S2D_DrawImage(img_bmp);
  S2D_DrawImage(img_r);
  S2D_DrawImage(img_g);
  S2D_DrawImage(img_b);

  // Sprites

  if (window->frames % 50 == 0) {
    if (sprite_step == 0) {
      S2D_ClipSprite(spr, 0, 0, 50, 50);
      sprite_step++;
    } else if (sprite_step == 1) {
      S2D_ClipSprite(spr, 50, 0, 50, 50);
      sprite_step++;
    } else if (sprite_step == 2) {
      S2D_ClipSprite(spr, 100, 0, 50, 50);
      sprite_step++;
    } else if (sprite_step == 3) {
      S2D_ClipSprite(spr, 150, 0, 50, 50);
      sprite_step = 0;
    }
  }

  S2D_DrawSprite(spr);

  // Text

  S2D_DrawText(txt_r);
  S2D_DrawText(txt_g);
  S2D_DrawText(txt_b);

  // Window stats

  S2D_DrawText(fps);
  if (window->frames % 20 == 0) S2D_SetText(fps_val, "%f", window->fps);
  S2D_DrawText(fps_val);

  // Mouse positions

  S2D_DrawQuad(pointer.x - 5, pointer.y - 7,  1, 1, 1, 1,
               pointer.x + 5, pointer.y - 7,  1, 1, 1, 1,
               pointer.x + 5, pointer.y + 4,  1, 1, 1, 1,
               pointer.x - 5, pointer.y + 4,  1, 1, 1, 1);

  if (mouse_click) {
    S2D_DrawQuad(click_pointer.x - 9, click_pointer.y - 11, 0, 1, 0, 1,
                 click_pointer.x + 9, click_pointer.y - 11, 0, 1, 0, 1,
                 click_pointer.x + 9, click_pointer.y + 8,  0, 1, 0, 1,
                 click_pointer.x - 9, click_pointer.y + 8,  0, 1, 0, 1);
    mouse_click = false;
  }
}


int main() {

  S2D_Diagnostics(true);

  window = S2D_CreateWindow("Simple 2D — Test Card", 600, 500, update, render, S2D_RESIZABLE);

  window->icon          = "media/s2d_icon.png";
  window->on_key        = on_key;
  window->on_mouse      = on_mouse;
  window->on_controller = on_controller;

  // Change viewport scaling modes:
  //   window->viewport.mode = S2D_FIXED;
  //   window->viewport.mode = S2D_EXPAND;
  //   window->viewport.mode = S2D_SCALE;  // Default
  //   window->viewport.mode = S2D_STRETCH;

  img_png = S2D_CreateImage("media/image.png");
  img_png->x = 300;
  img_png->y = 0;

  img_jpg = S2D_CreateImage("media/image.jpg");
  img_jpg->x = 400;
  img_jpg->y = 0;

  img_bmp = S2D_CreateImage("media/image.bmp");
  img_bmp->x = 500;
  img_bmp->y = 0;

  img_r = S2D_CreateImage("media/colors.png");
  img_r->x = 400;
  img_r->y = 200;
  img_r->width  = 50;
  img_r->height = 25;
  img_r->color.r = 1.0;
  img_r->color.g = 0.3;
  img_r->color.b = 0.3;
  img_r->color.a = 1.0;

  img_g = S2D_CreateImage("media/colors.png");
  img_g->x = 400;
  img_g->y = 225;
  img_g->width  = 25;
  img_g->height = 25;
  img_g->color.r = 0.3;
  img_g->color.g = 1.0;
  img_g->color.b = 0.3;
  img_g->color.a = 1.0;

  img_b = S2D_CreateImage("media/colors.png");
  img_b->x = 425;
  img_b->y = 225;
  img_b->width  = 25;
  img_b->height = 25;
  img_b->color.r = 0.3;
  img_b->color.g = 0.3;
  img_b->color.b = 1.0;
  img_b->color.a = 1.0;

  spr = S2D_CreateSprite("media/sprite_sheet.png");
  spr->x = 450;
  spr->y = 200;
  // Change size of sprite:
  //   spr->width  = 100;
  //   spr->height = 100;
  // Change color of sprite:
  //   spr->color.r = 1.0;
  //   spr->color.g = 1.0;
  //   spr->color.b = 1.0;
  //   spr->color.a = 1.0;

  txt_r = S2D_CreateText(font, "R", font_size);
  txt_r->x = 44;
  txt_r->y = 202;
  txt_r->color.r = 1.0;
  txt_r->color.g = 0.0;
  txt_r->color.b = 0.0;
  txt_r->color.a = 1.0;

  txt_g = S2D_CreateText(font, "G", font_size);
  txt_g->x = 92;
  txt_g->y = 202;
  txt_g->color.r = 0.0;
  txt_g->color.g = 1.0;
  txt_g->color.b = 0.0;
  txt_g->color.a = 1.0;

  txt_b = S2D_CreateText(font, "B", font_size);
  txt_b->x = 144;
  txt_b->y = 202;
  txt_b->color.r = 0.0;
  txt_b->color.g = 0.0;
  txt_b->color.b = 1.0;
  txt_b->color.a = 1.0;

  fps = S2D_CreateText(font, "FPS:", font_size);
  fps->x = 430;
  fps->y = 470;

  fps_val = S2D_CreateText(font, "", font_size);
  fps_val->x = 480;
  fps_val->y = 470;

  puts("Press `R` key to rotate textures.");

  S2D_Show(window);

  S2D_FreeWindow(window);

  return 0;
}
