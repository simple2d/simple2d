// controllers.c

#include "../include/simple2d.h"


/*
 * Load controller mappings from the specified file
 */
void S2D_LoadControllerMappingsFromFile(const char *path) {
  if (!S2D_FileExists(path)) {
    S2D_Log(S2D_ERROR, "Controller mappings file not found: %s", path);
    return;
  }
  int mappings_added = SDL_GameControllerAddMappingsFromFile(path);
  if (mappings_added == -1) {
    S2D_Error("S2D_LoadControllerMappingsFromFile", SDL_GetError());
  } else {
    S2D_Log(S2D_INFO, "Loaded %i controller mappings.", mappings_added);
  }
}


/*
 * Check if joystick is a controller
 */
bool S2D_IsController(SDL_JoystickID id) {
  return SDL_GameControllerFromInstanceID(id) == NULL ? false : true;
}


/*
 * Detect controllers and joysticks
 */
void S2D_DetectControllers() {

  if (SDL_NumJoysticks() > 0) {
    S2D_Log(S2D_INFO, "Controllers detected: %i", SDL_NumJoysticks());
  }

  // Enumerate joysticks
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {

    // Check to see if joystick supports SDL's game controller interface
    if (SDL_IsGameController(i)) {
      SDL_GameController *controller = SDL_GameControllerOpen(i);
      SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

      if (controller) {
        S2D_Log(S2D_INFO, "Controller #%i: %s", id, SDL_GameControllerName(controller));
      } else {
        S2D_Log(S2D_ERROR, "Could not open controller #%i: %s", id, SDL_GetError());
      }

    // Controller interface not supported, try to open as joystick
    } else {
      SDL_Joystick *joy = SDL_JoystickOpen(i);
      SDL_JoystickID id = SDL_JoystickInstanceID(joy);
      if (joy) {
        S2D_Log(S2D_INFO,
          "Controller #%i: %s\n      Axes: %d\n      Buttons: %d\n      Balls: %d",
          id, SDL_JoystickName(joy), SDL_JoystickNumAxes(joy),
          SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy)
        );
        S2D_Log(S2D_WARN, "Controller #%i does not have a mapping available", id);
      } else {
        S2D_Log(S2D_ERROR, "Could not open controller #%i", id);
      }
    }
  }
}
