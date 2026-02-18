#include <simple2d.h>

S2D_Window *window;
S2D_Image *controller;

double scale = 80;
double axis_LEFTX = 0;
double axis_LEFTY = 0;
double axis_RIGHTX = 0;
double axis_RIGHTY = 0;
double axis_LEFT_TRIGGER = 0;
double axis_RIGHT_TRIGGER = 0;
double axis_COUNT = 0;

bool btn_SOUTH = false;
bool btn_EAST = false;
bool btn_WEST = false;
bool btn_NORTH = false;
bool btn_BACK = false;
bool btn_GUIDE = false;
bool btn_START = false;
bool btn_LEFT_STICK = false;
bool btn_RIGHT_STICK = false;
bool btn_LEFT_SHOULDER = false;
bool btn_RIGHT_SHOULDER = false;
bool btn_DPAD_UP = false;
bool btn_DPAD_DOWN = false;
bool btn_DPAD_LEFT = false;
bool btn_DPAD_RIGHT = false;
bool btn_COUNT = false;


// Normalize axis values to 0.0...1.0
double to_d(int val) {
  return val > 0 ? val / 32767.0 : val / 32768.0;
}




void S2D_OnKey(S2D_Event e) {
  if (e.type != S2D_KEY_DOWN) return;
  if (strcmp(e.key, "Escape") == 0) {
    S2D_Close();
  }
}


void S2D_OnController(S2D_Event e) {
  puts("=== Controller Event ===");
  printf("Controller #%i\n", e.id);

  // Axes
  if (e.type == S2D_AXIS) {
    printf("Axis movement: #%i ", e.axis);
    switch (e.axis) {
      case S2D_AXIS_INVALID:
        S2D_Error("Controller", "Invalid axis!");
        break;
      case S2D_AXIS_LEFTX:
        printf("(S2D_AXIS_LEFTX)");
        axis_LEFTX = to_d(e.value) * scale;
        break;
      case S2D_AXIS_LEFTY:
        printf("(S2D_AXIS_LEFTY)");
        axis_LEFTY = to_d(e.value) * scale;
        break;
      case S2D_AXIS_RIGHTX:
        printf("(S2D_AXIS_RIGHTX)");
        axis_RIGHTX = to_d(e.value) * scale;
        break;
      case S2D_AXIS_RIGHTY:
        printf("(S2D_AXIS_RIGHTY)");
        axis_RIGHTY = to_d(e.value) * scale;
        break;
      case S2D_AXIS_LEFT_TRIGGER:
        printf("(S2D_AXIS_LEFT_TRIGGER)");
        axis_LEFT_TRIGGER = to_d(e.value) * scale;
        break;
      case S2D_AXIS_RIGHT_TRIGGER:
        printf("(S2D_AXIS_RIGHT_TRIGGER)");
        axis_RIGHT_TRIGGER = to_d(e.value) * scale;
        break;
      case S2D_AXIS_COUNT:
        printf("(S2D_AXIS_COUNT)");
        break;
    }
    printf(" Value: %i\n", e.value);

  // Buttons
  } else {
    switch (e.type) {
      case S2D_BUTTON_DOWN:
        printf("S2D_BUTTON_DOWN: #%i ", e.button);
        break;
      case S2D_BUTTON_UP:
        printf("S2D_BUTTON_UP: #%i ", e.button);
        break;
    }
    bool pressed = e.type == S2D_BUTTON_DOWN ? true : false;
    switch (e.button) {
      case S2D_BUTTON_INVALID:
        S2D_Error("Controller", "Invalid button!");
        break;
      case S2D_BUTTON_SOUTH:
        puts("(SOUTH)");
        btn_SOUTH = pressed;
        break;
      case S2D_BUTTON_EAST:
        puts("(EAST)");
        btn_EAST = pressed;
        break;
      case S2D_BUTTON_WEST:
        puts("(WEST)");
        btn_WEST = pressed;
        break;
      case S2D_BUTTON_NORTH:
        puts("(NORTH)");
        btn_NORTH = pressed;
        break;
      case S2D_BUTTON_BACK:
        puts("(BACK)");
        btn_BACK = pressed;
        break;
      case S2D_BUTTON_GUIDE:
        puts("(GUIDE)");
        btn_GUIDE = pressed;
        break;
      case S2D_BUTTON_START:
        puts("(START)");
        btn_START = pressed;
        break;
      case S2D_BUTTON_LEFT_STICK:
        puts("(LEFT_STICK)");
        btn_LEFT_STICK = pressed;
        break;
      case S2D_BUTTON_RIGHT_STICK:
        puts("(RIGHT_STICK)");
        btn_RIGHT_STICK = pressed;
        break;
      case S2D_BUTTON_LEFT_SHOULDER:
        puts("(LEFT_SHOULDER)");
        btn_LEFT_SHOULDER = pressed;
        break;
      case S2D_BUTTON_RIGHT_SHOULDER:
        puts("(RIGHT_SHOULDER)");
        btn_RIGHT_SHOULDER = pressed;
        break;
      case S2D_BUTTON_DPAD_UP:
        puts("(DPAD_UP)");
        btn_DPAD_UP = pressed;
        break;
      case S2D_BUTTON_DPAD_DOWN:
        puts("(DPAD_DOWN)");
        btn_DPAD_DOWN = pressed;
        break;
      case S2D_BUTTON_DPAD_LEFT:
        puts("(DPAD_LEFT)");
        btn_DPAD_LEFT = pressed;
        break;
      case S2D_BUTTON_DPAD_RIGHT:
        puts("(DPAD_RIGHT)");
        btn_DPAD_RIGHT = pressed;
        break;
      case S2D_BUTTON_COUNT:
        puts("(COUNT)");
        btn_COUNT = pressed;
        break;
    }
  }
}


void S2D_Init() {
  S2D_Diagnostics(true);

  window = S2D_CreateWindow(
    "Simple 2D — Controller", 600, 425
  );

  controller = S2D_CreateImage("media/controller.png");
}


void S2D_Render() {

  S2D_DrawImage(controller);

  // Axes

  S2D_DrawQuad(
    156,              130, 0, 1, 0, 1,
    156 + axis_LEFTX, 130, 0, 1, 0, 1,
    156 + axis_LEFTX, 159, 0, 1, 0, 1,
    156,              159, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    142, 145 + axis_LEFTY, 0, 1, 0, 1,
    171, 145 + axis_LEFTY, 0, 1, 0, 1,
    171, 145,              0, 1, 0, 1,
    142, 145,              0, 1, 0, 1
  );
  S2D_DrawQuad(
    374,               215, 0, 1, 0, 1,
    374 + axis_RIGHTX, 215, 0, 1, 0, 1,
    374 + axis_RIGHTX, 244, 0, 1, 0, 1,
    374,               244, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    359, 229 + axis_RIGHTY, 0, 1, 0, 1,
    388, 229 + axis_RIGHTY, 0, 1, 0, 1,
    388, 229,               0, 1, 0, 1,
    359, 229,               0, 1, 0, 1
  );
  S2D_DrawQuad(
     8, 71 - axis_LEFT_TRIGGER * .84, 0, 1, 0, 1,
    42, 71 - axis_LEFT_TRIGGER * .84, 0, 1, 0, 1,
    42, 71,                           0, 1, 0, 1,
     8, 71,                           0, 1, 0, 1
  );
  S2D_DrawQuad(
     8 + 550, 71 - axis_RIGHT_TRIGGER * .84, 0, 1, 0, 1,
    42 + 550, 71 - axis_RIGHT_TRIGGER * .84, 0, 1, 0, 1,
    42 + 550, 71,                            0, 1, 0, 1,
     8 + 550, 71,                            0, 1, 0, 1
  );

  // Buttons

  if (btn_SOUTH)  // green
    S2D_DrawQuad(
      426,      167,      0, 1, 0, 1,
      426 + 33, 167,      0, 1, 0, 1,
      426 + 33, 167 + 33, 0, 1, 0, 1,
      426,      167 + 33, 0, 1, 0, 1
    );
  if (btn_EAST)  // red
    S2D_DrawQuad(
      464,      129,      1, 0, 0, 1,
      464 + 33, 129,      1, 0, 0, 1,
      464 + 33, 129 + 33, 1, 0, 0, 1,
      464,      129 + 33, 1, 0, 0, 1
    );
  if (btn_WEST)  // blue
    S2D_DrawQuad(
      388,      128,      0, .7, 1, 1,
      388 + 33, 128,      0, .7, 1, 1,
      388 + 33, 128 + 33, 0, .7, 1, 1,
      388,      128 + 33, 0, .7, 1, 1
    );
  if (btn_NORTH)  // yellow
    S2D_DrawQuad(
      426,      91,      1, 1, 0, 1,
      426 + 33, 91,      1, 1, 0, 1,
      426 + 33, 91 + 33, 1, 1, 0, 1,
      426,      91 + 33, 1, 1, 0, 1
    );
  if (btn_BACK)
    S2D_DrawQuad(
      248,      133,      1, .5, 0, 1,
      248 + 23, 133,      1, .5, 0, 1,
      248 + 23, 133 + 23, 1, .5, 0, 1,
      248,      133 + 23, 1, .5, 0, 1
    );
  if (btn_GUIDE)
    S2D_DrawQuad(
      281,      69,      .5, 1, .5, 1,
      281 + 38, 69,      .5, 1, .5, 1,
      281 + 38, 69 + 38, .5, 1, .5, 1,
      281,      69 + 38, .5, 1, .5, 1
    );
  if (btn_START)
    S2D_DrawQuad(
      331,      133,      1, .5, 0, 1,
      331 + 23, 133,      1, .5, 0, 1,
      331 + 23, 133 + 23, 1, .5, 0, 1,
      331,      133 + 23, 1, .5, 0, 1
    );
  if (btn_LEFT_STICK)
    S2D_DrawQuad(
      134,      122,      1, 0, 0, 1,
      134 + 45, 122,      1, 0, 0, 1,
      134 + 45, 122 + 45, 1, 0, 0, 1,
      134,      122 + 45, 1, 0, 0, 1
    );
  if (btn_RIGHT_STICK)
    S2D_DrawQuad(
      351,      207,      1, 0, 0, 1,
      351 + 45, 207,      1, 0, 0, 1,
      351 + 45, 207 + 45, 1, 0, 0, 1,
      351,      207 + 45, 1, 0, 0, 1
    );
  if (btn_LEFT_SHOULDER)
    S2D_DrawQuad(
      111, 84, .5, 0, 1, 1,
      117, 64, .5, 0, 1, 1,
      198, 39, .5, 0, 1, 1,
      225, 52, .5, 0, 1, 1
    );
  if (btn_RIGHT_SHOULDER)
    S2D_DrawQuad(
      494, 85, .5, 0, 1, 1,
      484, 64, .5, 0, 1, 1,
      401, 39, .5, 0, 1, 1,
      378, 51, .5, 0, 1, 1
    );
  if (btn_DPAD_UP)
    S2D_DrawQuad(
      216,      194,      1, 0, .5, 1,
      216 + 23, 194,      1, 0, .5, 1,
      216 + 23, 194 + 28, 1, 0, .5, 1,
      216,      194 + 28, 1, 0, .5, 1
    );
  if (btn_DPAD_DOWN)
    S2D_DrawQuad(
      216,      243,      1, 0, .5, 1,
      216 + 23, 243,      1, 0, .5, 1,
      216 + 23, 243 + 27, 1, 0, .5, 1,
      216,      243 + 27, 1, 0, .5, 1
    );
  if (btn_DPAD_LEFT)
    S2D_DrawQuad(
      189,      221,      1, 0, .5, 1,
      189 + 28, 221,      1, 0, .5, 1,
      189 + 28, 221 + 22, 1, 0, .5, 1,
      189,      221 + 22, 1, 0, .5, 1
    );
  if (btn_DPAD_RIGHT)
    S2D_DrawQuad(
      238,      221,      1, 0, .5, 1,
      238 + 28, 221,      1, 0, .5, 1,
      238 + 28, 221 + 22, 1, 0, .5, 1,
      238,      221 + 22, 1, 0, .5, 1
    );
}
