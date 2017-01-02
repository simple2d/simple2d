#include <simple2d.h>

S2D_Window *window;

double axis0 = 0;
double axis1 = 0;
double axis2 = 0;
double axis3 = 0;
double axis4 = 0;
double axis5 = 0;

bool btn0  = false;
bool btn1  = false;
bool btn2  = false;
bool btn3  = false;
bool btn4  = false;
bool btn5  = false;
bool btn6  = false;
bool btn7  = false;
bool btn8  = false;
bool btn9  = false;
bool btn10 = false;
bool btn11 = false;
bool btn12 = false;
bool btn13 = false;
bool btn14 = false;
bool btn15 = false;


double to_d(int val) {
  if (val > 0) {
    return val / 32767.0;
  } else {
    return val / 32768.0;
  }
}


void render() {
  
  // Axises
  
  S2D_DrawQuad(
    300,          0, 0, 1, 0, 1,
    300 + axis0,  0, 0, 1, 0, 1,
    300 + axis0, 50, 0, 1, 0, 1,
    300,         50, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    300,          50, 0, 1, 0, 1,
    300 + axis1,  50, 0, 1, 0, 1,
    300 + axis1, 100, 0, 1, 0, 1,
    300,         100, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    300,         100, 0, 1, 0, 1,
    300 + axis2, 100, 0, 1, 0, 1,
    300 + axis2, 150, 0, 1, 0, 1,
    300,         150, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    300,         150, 0, 1, 0, 1,
    300 + axis3, 150, 0, 1, 0, 1,
    300 + axis3, 200, 0, 1, 0, 1,
    300,         200, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    300,         200, 0, 1, 0, 1,
    300 + axis4, 200, 0, 1, 0, 1,
    300 + axis4, 250, 0, 1, 0, 1,
    300,         250, 0, 1, 0, 1
  );
  S2D_DrawQuad(
    300,         250, 0, 1, 0, 1,
    300 + axis5, 250, 0, 1, 0, 1,
    300 + axis5, 300, 0, 1, 0, 1,
    300,         300, 0, 1, 0, 1
  );
  
  // Buttons
  
  // Cell 1,1
  if (btn0)
    S2D_DrawQuad(
       0, 300, 0, 1, 0, 1,
      50, 300, 0, 1, 0, 1,
      50, 350, 0, 1, 0, 1,
       0, 350, 0, 1, 0, 1
    );
  // Cell 1,2
  if (btn1)
    S2D_DrawQuad(
       50, 300, 1, 0, 0, 1,
      100, 300, 1, 0, 0, 1,
      100, 350, 1, 0, 0, 1,
       50, 350, 1, 0, 0, 1
    );
  // Cell 1,3
  if (btn2)
    S2D_DrawQuad(
      100, 300, 0, 0, 1, 1,
      150, 300, 0, 0, 1, 1,
      150, 350, 0, 0, 1, 1,
      100, 350, 0, 0, 1, 1
    );
  // Cell 1,4
  if (btn3)
    S2D_DrawQuad(
      150, 300, 1, 1, 0, 1,
      200, 300, 1, 1, 0, 1,
      200, 350, 1, 1, 0, 1,
      150, 350, 1, 1, 0, 1
    );
  // Cell 1,5
  if (btn4)
    S2D_DrawQuad(
      200, 300, 1, 0, 1, 1,
      250, 300, 1, 0, 1, 1,
      250, 350, 1, 0, 1, 1,
      200, 350, 1, 0, 1, 1
    );
  // Cell 1,6
  if (btn5)
    S2D_DrawQuad(
      250, 300, 1, 0, 1, 1,
      300, 300, 1, 0, 1, 1,
      300, 350, 1, 0, 1, 1,
      250, 350, 1, 0, 1, 1
    );
  // Cell 1,7
  if (btn6)
    S2D_DrawQuad(
      300, 300, 0, 1, 1, 1,
      350, 300, 0, 1, 1, 1,
      350, 350, 0, 1, 1, 1,
      300, 350, 0, 1, 1, 1
    );
  // Cell 1,8
  if (btn7)
    S2D_DrawQuad(
      350, 300, 0, 1, 1, 1,
      400, 300, 0, 1, 1, 1,
      400, 350, 0, 1, 1, 1,
      350, 350, 0, 1, 1, 1
    );
  // Cell 1,9
  if (btn8)
    S2D_DrawQuad(
      400, 300, 1, 0.5, 0, 1,
      450, 300, 1, 0.5, 0, 1,
      450, 350, 1, 0.5, 0, 1,
      400, 350, 1, 0.5, 0, 1
    );
  // Cell 1,10
  if (btn9)
    S2D_DrawQuad(
      450, 300, 1, 0.5, 0, 1,
      500, 300, 1, 0.5, 0, 1,
      500, 350, 1, 0.5, 0, 1,
      450, 350, 1, 0.5, 0, 1
    );
  // Cell 1,11
  if (btn10)
    S2D_DrawQuad(
      500, 300, 1, 0.5, 0, 1,
      550, 300, 1, 0.5, 0, 1,
      550, 350, 1, 0.5, 0, 1,
      500, 350, 1, 0.5, 0, 1
    );
  // Cell 2,1
  if (btn11)
    S2D_DrawQuad(
       0, 350, 0, 0.5, 1, 1,
      50, 350, 0, 0.5, 1, 1,
      50, 400, 0, 0.5, 1, 1,
       0, 400, 0, 0.5, 1, 1
    );
  // Cell 2,2
  if (btn12)
    S2D_DrawQuad(
       50, 350, 0, 0.5, 1, 1,
      100, 350, 0, 0.5, 1, 1,
      100, 400, 0, 0.5, 1, 1,
       50, 400, 0, 0.5, 1, 1
    );
  // Cell 2,3
  if (btn13)
    S2D_DrawQuad(
      100, 350, 0, 0.5, 1, 1,
      150, 350, 0, 0.5, 1, 1,
      150, 400, 0, 0.5, 1, 1,
      100, 400, 0, 0.5, 1, 1
    );
  // Cell 2,4
  if (btn14)
    S2D_DrawQuad(
      150, 350, 0, 0.5, 1, 1,
      200, 350, 0, 0.5, 1, 1,
      200, 400, 0, 0.5, 1, 1,
      150, 400, 0, 0.5, 1, 1
    );
  
  // Vertical axis line
  
  S2D_DrawQuad(
    300,   0, 1, 0, 0, 1,
    301,   0, 1, 0, 0, 1,
    301, 300, 1, 0, 0, 1,
    300, 300, 1, 0, 0, 1
  );
  
  // Button grid
  
  S2D_DrawQuad(
      0, 300, 1, 1, 1, 1,
    550, 300, 1, 1, 1, 1,
    550, 301, 1, 1, 1, 1,
      0, 301, 1, 1, 1, 1
  );
  S2D_DrawQuad(
      0, 350, 1, 1, 1, 1,
    550, 350, 1, 1, 1, 1,
    550, 351, 1, 1, 1, 1,
      0, 351, 1, 1, 1, 1
  );
  S2D_DrawQuad(
      0, 399, 1, 1, 1, 1,
    200, 399, 1, 1, 1, 1,
    200, 400, 1, 1, 1, 1,
      0, 400, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    0, 300, 1, 1, 1, 1,
    0, 400, 1, 1, 1, 1,
    1, 400, 1, 1, 1, 1,
    1, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    50, 300, 1, 1, 1, 1,
    50, 400, 1, 1, 1, 1,
    51, 400, 1, 1, 1, 1,
    51, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    100, 300, 1, 1, 1, 1,
    100, 400, 1, 1, 1, 1,
    101, 400, 1, 1, 1, 1,
    101, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    150, 300, 1, 1, 1, 1,
    150, 400, 1, 1, 1, 1,
    151, 400, 1, 1, 1, 1,
    151, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    200, 300, 1, 1, 1, 1,
    200, 400, 1, 1, 1, 1,
    201, 400, 1, 1, 1, 1,
    201, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    250, 300, 1, 1, 1, 1,
    250, 350, 1, 1, 1, 1,
    251, 350, 1, 1, 1, 1,
    251, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    300, 300, 1, 1, 1, 1,
    300, 350, 1, 1, 1, 1,
    301, 350, 1, 1, 1, 1,
    301, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    350, 300, 1, 1, 1, 1,
    350, 350, 1, 1, 1, 1,
    351, 350, 1, 1, 1, 1,
    351, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    400, 300, 1, 1, 1, 1,
    400, 350, 1, 1, 1, 1,
    401, 350, 1, 1, 1, 1,
    401, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    450, 300, 1, 1, 1, 1,
    450, 350, 1, 1, 1, 1,
    451, 350, 1, 1, 1, 1,
    451, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    500, 300, 1, 1, 1, 1,
    500, 350, 1, 1, 1, 1,
    501, 350, 1, 1, 1, 1,
    501, 300, 1, 1, 1, 1
  );
  S2D_DrawQuad(
    550, 300, 1, 1, 1, 1,
    550, 350, 1, 1, 1, 1,
    551, 350, 1, 1, 1, 1,
    551, 300, 1, 1, 1, 1
  );
}


void on_key(S2D_Event e, const char *key) {
  if (e != S2D_KEYDOWN) return;
  
  if (strcmp(key, "Escape") == 0) {
    S2D_Close(window);
  }
  
  if (strcmp(key, "D") == 0) {
    S2D_DetectControllers();
  }
}


void on_controller(int which, bool is_axis, int axis, int val, bool is_btn, int btn, bool pressed) {
  
  puts("=== Controller Pressed ===");
  printf(
    "which: %i\nis_axis: %i\naxis: %i\nval: %i\nis_btn: %i\nbtn: %i\n",
    which, is_axis, axis, val, is_btn, btn
  );
  
  double scale = window->viewport.width / 2.0;
  
  if (is_axis) {
    switch (axis) {
      case 0:
        axis0 = to_d(val) * scale;
        break;
      case 1:
        axis1 = to_d(val) * scale;
        break;
      case 2:
        axis2 = to_d(val) * scale;
        break;
      case 3:
        axis3 = to_d(val) * scale;
        break;
      case 4:
        axis4 = to_d(val) * scale;
        break;
      case 5:
        axis5 = to_d(val) * scale;
        break;
      default:
        puts("No axis match");
    }
  }
  
  if (is_btn) {
    switch (btn) {
      case 0:
        btn0 = pressed;
        break;
      case 1:
        btn1 = pressed;
        break;
      case 2:
        btn2 = pressed;
        break;
      case 3:
        btn3 = pressed;
        break;
      case 4:
        btn4 = pressed;
        break;
      case 5:
        btn5 = pressed;
        break;
      case 6:
        btn6 = pressed;
        break;
      case 7:
        btn7 = pressed;
        break;
      case 8:
        btn8 = pressed;
        break;
      case 9:
        btn9 = pressed;
        break;
      case 10:
        btn10 = pressed;
        break;
      case 11:
        btn11 = pressed;
        break;
      case 12:
        btn12 = pressed;
        break;
      case 13:
        btn13 = pressed;
        break;
      case 14:
        btn14 = pressed;
        break;
      case 15:
        btn15 = pressed;
        break;
      default:
        puts("No button match");
    }
  }
}


int main() {
  
  S2D_Diagnostics(true);
  
  window = S2D_CreateWindow(
    "Controller Test", 600, 400, NULL, render, 0
  );
  
  window->on_key        = on_key;
  window->on_controller = on_controller;
  
  S2D_Show(window);
  S2D_FreeWindow(window);
  return 0;
}
