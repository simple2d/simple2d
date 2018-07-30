// window.c

#include "../include/simple2d.h"


/*
 * Create a window
 */
S2D_Window *S2D_CreateWindow(const char *title, int width, int height,
                             S2D_Update update, S2D_Render render, int flags) {

  S2D_Init();

  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  S2D_Log(S2D_INFO, "Current display mode is %dx%dpx @ %dhz", dm.w, dm.h, dm.refresh_rate);

  width  = width  == S2D_DISPLAY_WIDTH  ? dm.w : width;
  height = height == S2D_DISPLAY_HEIGHT ? dm.h : height;

  // Allocate window and set default values
  S2D_Window *window      = (S2D_Window *) malloc(sizeof(S2D_Window));
  window->sdl             = NULL;
  window->glcontext       = NULL;
  window->title           = title;
  window->width           = width;
  window->height          = height;
  window->orig_width      = width;
  window->orig_height     = height;
  window->viewport.width  = width;
  window->viewport.height = height;
  window->viewport.mode   = S2D_SCALE;
  window->update          = update;
  window->render          = render;
  window->flags           = flags;
  window->on_key          = NULL;
  window->on_mouse        = NULL;
  window->on_controller   = NULL;
  window->vsync           = true;
  window->fps_cap         = 60;
  window->background.r    = 0.0;
  window->background.g    = 0.0;
  window->background.b    = 0.0;
  window->background.a    = 1.0;
  window->close           = true;

  // Return the window structure
  return window;
}


/*
 * Show the window
 */
int S2D_Show(S2D_Window *window) {

  if (!window) {
    S2D_Error("S2D_Show", "Window cannot be shown (because it's NULL)");
    return 1;
  }

  // Create SDL window
  window->sdl = SDL_CreateWindow(
    window->title,                                   // title
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  // window position
    window->width, window->height,                   // window size
    SDL_WINDOW_OPENGL | window->flags                // flags
  );

  if (!window->sdl) S2D_Error("SDL_CreateWindow", SDL_GetError());

  // The window created by SDL might not actually be the requested size.
  // If it's not the same, retrieve and store the actual window size.
  int actual_width, actual_height;
  SDL_GetWindowSize(window->sdl, &actual_width, &actual_height);

  if ((window->width != actual_width) || (window->height != actual_height)) {
    S2D_Log(S2D_INFO,
      "Scaling window to %ix%i (requested size was %ix%i)",
      actual_width, actual_height, window->width, window->height
    );
    window->width  = actual_width;
    window->height = actual_height;
    window->orig_width  = actual_width;
    window->orig_height = actual_height;
  }

  // Set Up OpenGL /////////////////////////////////////////////////////////////

  S2D_GL_Init(window);

  // Set Main Loop Data ////////////////////////////////////////////////////////

  const Uint8 *key_state;

  Uint32 frames = 0;           // Total frames since start
  Uint32 frames_last_sec = 0;  // Frames in the last second
  Uint32 start_ms = SDL_GetTicks();  // Elapsed time since start
  Uint32 next_second_ms = SDL_GetTicks(); // The last time plus a second
  Uint32 begin_ms = start_ms;  // Time at beginning of loop
  Uint32 end_ms;               // Time at end of loop
  Uint32 elapsed_ms;           // Total elapsed time
  Uint32 loop_ms;              // Elapsed time of loop
  int delay_ms;                // Amount of delay to achieve desired frame rate
  const double decay_rate = 0.5;  // Determines how fast an average decays over time
  double fps = window->fps_cap;   // Moving average of actual FPS, initial value a guess

  // Enable VSync
  if (window->vsync) {
    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
      S2D_Log(S2D_WARN, "VSync cannot be enabled");
    }
  }

  window->close = false;

  // Main Loop /////////////////////////////////////////////////////////////////

  while (!window->close) {

    // Clear Frame /////////////////////////////////////////////////////////////

    S2D_GL_Clear(window->background);

    // Set FPS /////////////////////////////////////////////////////////////////

    frames++;
    frames_last_sec++;
    end_ms = SDL_GetTicks();
    elapsed_ms = end_ms - start_ms;

    // Calculate the frame rate using an exponential moving average
    if (next_second_ms < end_ms) {
      fps = decay_rate * fps + (1.0 - decay_rate) * frames_last_sec;
      frames_last_sec = 0;
      next_second_ms = SDL_GetTicks() + 1000;
    }

    loop_ms = end_ms - begin_ms;
    delay_ms = (1000 / window->fps_cap) - loop_ms;

    if (delay_ms < 0) delay_ms = 0;

    // Note: `loop_ms + delay_ms` should equal `1000 / fps_cap`

    SDL_Delay(delay_ms);
    begin_ms = SDL_GetTicks();

    // Handle Input ////////////////////////////////////////////////////////////

    int mx, my;  // mouse x, y coordinates

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {

        case SDL_KEYDOWN:
          if (window->on_key && e.key.repeat == 0) {
            S2D_Event event = {
              .type = S2D_KEY_DOWN, .key = SDL_GetScancodeName(e.key.keysym.scancode)
            };
            window->on_key(event);
          }
          break;

        case SDL_KEYUP:
          if (window->on_key) {
            S2D_Event event = {
              .type = S2D_KEY_UP, .key = SDL_GetScancodeName(e.key.keysym.scancode)
            };
            window->on_key(event);
          }
          break;

        case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
          if (window->on_mouse) {
            S2D_GetMouseOnViewport(window, e.button.x, e.button.y, &mx, &my);
            S2D_Event event = {
              .button = e.button.button, .x = mx, .y = my
            };
            event.type = e.type == SDL_MOUSEBUTTONDOWN ? S2D_MOUSE_DOWN : S2D_MOUSE_UP;
            event.dblclick = e.button.clicks == 2 ? true : false;
            window->on_mouse(event);
          }
          break;

        case SDL_MOUSEWHEEL:
          if (window->on_mouse) {
            S2D_Event event = {
              .type = S2D_MOUSE_SCROLL, .direction = e.wheel.direction,
              .delta_x = e.wheel.x, .delta_y = -e.wheel.y
            };
            window->on_mouse(event);
          }
          break;

        case SDL_MOUSEMOTION:
          if (window->on_mouse) {
            S2D_GetMouseOnViewport(window, e.motion.x, e.motion.y, &mx, &my);
            S2D_Event event = {
              .type = S2D_MOUSE_MOVE,
              .x = mx, .y = my, .delta_x = e.motion.xrel, .delta_y = e.motion.yrel
            };
            window->on_mouse(event);
          }
          break;

        case SDL_CONTROLLERAXISMOTION:
          if (window->on_controller) {
            S2D_Event event = {
              .which = e.caxis.which, .type = S2D_AXIS,
              .axis = e.caxis.axis, .value = e.caxis.value
            };
            window->on_controller(event);
          }
          break;

        case SDL_JOYAXISMOTION:
          if (window->on_controller && !S2D_IsController(e.jbutton.which)) {
            S2D_Event event = {
              .which = e.jaxis.which, .type = S2D_AXIS,
              .axis = e.jaxis.axis, .value = e.jaxis.value
            };
            window->on_controller(event);
          }
          break;

        case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
          if (window->on_controller) {
            S2D_Event event = {
              .which = e.cbutton.which, .button = e.cbutton.button
            };
            event.type = e.type == SDL_CONTROLLERBUTTONDOWN ? S2D_BUTTON_DOWN : S2D_BUTTON_UP;
            window->on_controller(event);
          }
          break;

        case SDL_JOYBUTTONDOWN: case SDL_JOYBUTTONUP:
          if (window->on_controller && !S2D_IsController(e.jbutton.which)) {
            S2D_Event event = {
              .which = e.jbutton.which, .button = e.jbutton.button
            };
            event.type = e.type == SDL_JOYBUTTONDOWN ? S2D_BUTTON_DOWN : S2D_BUTTON_UP;
            window->on_controller(event);
          }
          break;

        case SDL_CONTROLLERDEVICEADDED:
          if (SDL_IsGameController(e.cdevice.which)) S2D_DetectControllers();
          break;

        case SDL_JOYDEVICEADDED:
          if (!SDL_IsGameController(e.jdevice.which)) S2D_DetectControllers();
          break;

        case SDL_CONTROLLERDEVICEREMOVED:
          SDL_GameControllerClose(SDL_GameControllerFromInstanceID(e.cdevice.which));
          S2D_Log(S2D_INFO, "Controller #%i removed", e.cdevice.which);
          break;

        case SDL_JOYDEVICEREMOVED:
          if (!S2D_IsController(e.jdevice.which)) {
            SDL_JoystickClose(SDL_JoystickFromInstanceID(e.jdevice.which));
          }
          break;

        case SDL_WINDOWEVENT:
          switch (e.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
              // Store new window size, set viewport
              window->width  = e.window.data1;
              window->height = e.window.data2;
              S2D_GL_SetViewport(window);
              break;
          }
          break;

        case SDL_QUIT:
          S2D_Close(window);
          break;
      }
    }

    // Detect keys held down
    int num_keys;
    key_state = SDL_GetKeyboardState(&num_keys);

    for (int i = 0; i < num_keys; i++) {
      if (window->on_key) {
        if (key_state[i] == 1) {
          S2D_Event event = {
            .type = S2D_KEY_HELD, .key = SDL_GetScancodeName(i)
          };
          window->on_key(event);
        }
      }
    }

    // Get and store mouse position relative to the viewport
    int wx, wy;  // mouse x, y coordinates relative to the window
    SDL_GetMouseState(&wx, &wy);
    S2D_GetMouseOnViewport(window, wx, wy, &window->mouse.x, &window->mouse.y);

    // Update Window State /////////////////////////////////////////////////////

    // Store new values in the window
    window->frames     = frames;
    window->elapsed_ms = elapsed_ms;
    window->loop_ms    = loop_ms;
    window->delay_ms   = delay_ms;
    window->fps        = fps;

    // Call update and render callbacks
    if (window->update) window->update();
    if (window->render) window->render();

    // Draw Frame //////////////////////////////////////////////////////////////
    SDL_GL_SwapWindow(window->sdl);
  }

  return 0;
}


/*
 * Close the window
 */
int S2D_Close(S2D_Window *window) {
  if (!window->close) {
    S2D_Log(S2D_INFO, "Closing window");
    window->close = true;
  }
  return 0;
}


/*
 * Free all resources
 */
int S2D_FreeWindow(S2D_Window *window) {
  S2D_Close(window);
  SDL_GL_DeleteContext(window->glcontext);
  SDL_DestroyWindow(window->sdl);
  free(window);
  return 0;
}
