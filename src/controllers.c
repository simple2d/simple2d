// controllers.c

#include "../include/simple2d.h"


/*
 * Detect controllers and joysticks
 */
void S2D_DetectControllers() {

  if (SDL_NumJoysticks() > 0) {
    S2D_Log(S2D_INFO, "Controllers detected: %i", SDL_NumJoysticks());
  }

  // Variables for controllers and joysticks
  SDL_GameController *controller = NULL;
  SDL_Joystick *joy = NULL;

  // Enumerate joysticks
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {

    // Check to see if joystick supports SDL's game controller interface
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
      if (controller) {
        S2D_Log(S2D_INFO, "Controller #%i: %s", i, SDL_GameControllerName(controller));
      } else {
        S2D_Log(S2D_ERROR, "Could not open controller #%i: %s", i, SDL_GetError());
      }

    // Controller interface not supported, try to open as joystick
    } else {
      joy = SDL_JoystickOpen(i);
      if (joy) {
        S2D_Log(S2D_INFO,
          "Controller #%i (generic): %s\n      Axes: %d\n      Buttons: %d\n      Balls: %d",
          i, SDL_JoystickName(joy), SDL_JoystickNumAxes(joy),
          SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy));
      } else {
        S2D_Log(S2D_ERROR, "Could not open generic controller #%i", i);
      }
    }
  }
}
