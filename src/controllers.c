// controllers.c

#include "../include/simple2d.h"


/*
 * Detect controllers and joysticks
 */
void S2D_DetectControllers() {
  
  if (SDL_NumJoysticks() > 0) {
    sprintf(S2D_msg, "Joysticks detected: %i", SDL_NumJoysticks());
    S2D_Log(S2D_msg, S2D_INFO);
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
        sprintf(S2D_msg, "Found a valid controller, named: %s\n",
                SDL_GameControllerName(controller));
        S2D_Log(S2D_msg, S2D_INFO);
      } else {
        sprintf(S2D_msg, "Could not open game controller %i: %s\n", i, SDL_GetError());
        S2D_Log(S2D_msg, S2D_ERROR);
      }
      
    // Controller interface not supported, try to open as joystick
    } else {
      sprintf(S2D_msg, "Joystick %i is not supported by the game controller interface", i);
      S2D_Log(S2D_msg, S2D_WARN);
      joy = SDL_JoystickOpen(i);
      
      // Joystick is valid
      if (joy) {
        sprintf(S2D_msg,
          "Opened Joystick %i\n"
          "Name: %s\n"
          "Axes: %d\n"
          "Buttons: %d\n"
          "Balls: %d",
          i, SDL_JoystickName(joy), SDL_JoystickNumAxes(joy),
          SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy)
        );
        S2D_Log(S2D_msg, S2D_INFO);
        
      // Joystick not valid
      } else {
        sprintf(S2D_msg, "Could not open Joystick %i", i);
        S2D_Log(S2D_msg, S2D_ERROR);
      }
    }
  }
}
