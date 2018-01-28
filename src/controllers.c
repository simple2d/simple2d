// controllers.c

#include "../include/simple2d.h"


/*
 * Add controller mapping from string
 */
void S2D_AddControllerMapping(const char *map) {
  int result = SDL_GameControllerAddMapping(map);

  char guid[33];
  strncpy(guid, map, 32);

  switch (result) {
    case 1:
      S2D_Log(S2D_INFO, "Mapping added for GUID: %s", guid);
      break;
    case 0:
      S2D_Log(S2D_INFO, "Mapping updated for GUID: %s", guid);
      break;
    case -1:
      S2D_Error("SDL_GameControllerAddMapping", SDL_GetError());
      break;
  }
}


/*
 * Load controller mappings from the specified file
 */
void S2D_LoadControllerMappingsFromFile(const char *path) {
  if (!S2D_FileExists(path)) {
    S2D_Log(S2D_WARN, "Controller mappings file not found: %s", path);
    return;
  }

  int mappings_added = SDL_GameControllerAddMappingsFromFile(path);
  if (mappings_added == -1) {
    S2D_Error("SDL_GameControllerAddMappingsFromFile", SDL_GetError());
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

  SDL_JoystickGUID guid;
  char guid_str[33];

  if (SDL_NumJoysticks() > 0) {
    S2D_Log(S2D_INFO, "Controllers detected: %i", SDL_NumJoysticks());
  }

  // Enumerate joysticks
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {

    // Check to see if joystick supports SDL's game controller interface
    if (SDL_IsGameController(i)) {
      SDL_GameController *controller = SDL_GameControllerOpen(i);
      SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

      SDL_JoystickGetGUIDString(
        SDL_JoystickGetGUID(SDL_GameControllerGetJoystick(controller)),
        guid_str, 33
      );

      if (controller) {
        S2D_Log(S2D_INFO, "Controller #%i: %s\n      GUID: %s", id, SDL_GameControllerName(controller), guid_str);
      } else {
        S2D_Log(S2D_ERROR, "Could not open controller #%i: %s", id, SDL_GetError());
      }

    // Controller interface not supported, try to open as joystick
    } else {
      SDL_Joystick *joy = SDL_JoystickOpen(i);
      SDL_JoystickID id = SDL_JoystickInstanceID(joy);

      if (joy) {
        SDL_JoystickGetGUIDString(
          SDL_JoystickGetGUID(joy),
          guid_str, 33
        );
        S2D_Log(S2D_INFO,
          "Controller #%i: %s\n      GUID: %s\n      Axes: %d\n      Buttons: %d\n      Balls: %d",
          id, SDL_JoystickName(joy), guid_str, SDL_JoystickNumAxes(joy),
          SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy)
        );
        S2D_Log(S2D_WARN, "Controller #%i does not have a mapping available", id);
      } else {
        S2D_Log(S2D_ERROR, "Could not open controller #%i", id);
      }
    }
  }
}
