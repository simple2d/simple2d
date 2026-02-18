// Simple 2D Shared functions and data

#include "../include/simple2d.h"


// Global Simple 2D app instance
S2D_App s2d_app;

// Default implementations of user-defined S2D functions
__attribute__((weak)) void S2D_Init() {}
__attribute__((weak)) void S2D_OnKey(S2D_Event e) {}
__attribute__((weak)) void S2D_OnMouse(S2D_Event e) {}
__attribute__((weak)) void S2D_OnController(S2D_Event e) {}
__attribute__((weak)) void S2D_Update() {}
__attribute__((weak)) void S2D_Render() {}
__attribute__((weak)) void S2D_Quit() {}


/*
 * Provide a `vasprintf()` implementation for Windows
 */
#ifdef _WIN32
int vasprintf(char **strp, const char *fmt, va_list ap) {
  int r = -1, size = _vscprintf(fmt, ap);
  if ((size >= 0) && (size < INT_MAX)) {
    *strp = (char *)malloc(size + 1);
    if (*strp) {
      r = vsnprintf(*strp, size + 1, fmt, ap);
      if (r == -1) free(*strp);
    }
  } else { *strp = 0; }
  return(r);
}
#endif


/*
 * Checks if a file exists and can be accessed
 */
bool S2D_FileExists(const char *path) {
  if (!path) return false;
  return access(path, F_OK) == 0;
}


/*
 * Logs standard messages to the console
 */
void S2D_Log(int type, const char *msg, ...) {

  // Always log if diagnostics set, or if a warning or error message
  if (s2d_app.diagnostics || type != S2D_INFO) {

    va_list args;
    va_start(args, msg);

    switch (type) {
      case S2D_INFO:
        printf("\033[1;36mInfo:\033[0m ");
        break;
      case S2D_WARN:
        printf("\033[1;33mWarning:\033[0m ");
        break;
      case S2D_ERROR:
        printf("\033[1;31mError:\033[0m ");
        break;
    }

    vprintf(msg, args);
    printf("\n");
    va_end(args);
  }
}


/*
 * Logs Simple 2D errors to the console, with caller and message body
 */
void S2D_Error(const char *caller, const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  char *fmsg;
  vasprintf(&fmsg, msg, args);
  S2D_Log(S2D_ERROR, "(%s) %s", caller, fmsg);
  free(fmsg);
  va_end(args);
}


/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status) {
  s2d_app.diagnostics = status;
}


/*
 * Enable terminal colors in Windows
 */
void S2D_Windows_EnableTerminalColors() {
  #if defined(_WIN32)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  #endif
}


/*
 * Returns true if the given S2D_Event's key matches the specified key string.\
 */
bool S2D_KeyIs(S2D_Event e, const char *key) {
  return strcmp(e.key, key) == 0;
}


/*
 * Calculates and returns the average frames per second (FPS) over the last
 * FPS_SAMPLE_COUNT frames. Uses a circular buffer to store frame times and
 * computes the rolling average FPS based on the elapsed time between frames.
 */
 #define FPS_SAMPLE_COUNT 60  // Number of frames to average
 static double frame_times[FPS_SAMPLE_COUNT];
 static int frame_index = 0;
 static int frame_count = 0;
 static SDL_Time last_time;
 static int fps_initialized = 0;

 double get_frame_rate() {
   SDL_Time current_time;
   double avg_fps = 0.0;

   if (!SDL_GetCurrentTime(&current_time)) {
     // Handle error, e.g., return 0 or a negative value
     return 0.0;
   }

   if (!fps_initialized) {
     last_time = current_time;
     fps_initialized = 1;
   } else {
     // Calculate delta time in seconds
     double delta = (current_time - last_time) / 1e9;

     // Store frame time in circular buffer
     frame_times[frame_index] = delta;
     frame_index = (frame_index + 1) % FPS_SAMPLE_COUNT;
     if (frame_count < FPS_SAMPLE_COUNT) frame_count++;

     // Calculate rolling average FPS
     double total_time = 0.0;
     for (int i = 0; i < frame_count; i++) {
       total_time += frame_times[i];
     }
     avg_fps = frame_count / total_time;

     last_time = current_time;
   }

   return avg_fps;
 }


/*
 * Draws the frame rate in the window
 */
void draw_frame_rate() {
  // Draw FPS directly on screen using a surface (no fonts)
  int scale = (s2d_app.window->display_scale > 1) ? 6 : 3; // Set scale for larger text

  int digit_width = 3 * scale;
  int digit_height = 5 * scale;
  int char_spacing = scale; // Space between digits
  // Convert FPS to string
  char fps_str[16];
  snprintf(fps_str, sizeof(fps_str), "%.0f", s2d_app.window->fps);

  // Calculate width based on number of digits
  int len = strlen(fps_str);
  int fps_width = len * digit_width + (len - 1) * char_spacing + 8;
  int fps_height = digit_height + 10;

  SDL_Surface *fps_surface = SDL_CreateSurface(fps_width, fps_height, SDL_PIXELFORMAT_RGBA32);

  if (fps_surface) {
    // Clear surface with semi-transparent black background
    SDL_FillSurfaceRect(fps_surface, NULL, SDL_MapRGBA(SDL_GetPixelFormatDetails(fps_surface->format), NULL, 0, 0, 0, 180));

    // Simple 3x5 digit patterns (each digit is 3 wide, 5 tall)
    static const uint8_t digits[10][5] = {
      {0b111, 0b101, 0b101, 0b101, 0b111}, // 0
      {0b010, 0b110, 0b010, 0b010, 0b111}, // 1
      {0b111, 0b001, 0b111, 0b100, 0b111}, // 2
      {0b111, 0b001, 0b111, 0b001, 0b111}, // 3
      {0b101, 0b101, 0b111, 0b001, 0b001}, // 4
      {0b111, 0b100, 0b111, 0b001, 0b111}, // 5
      {0b111, 0b100, 0b111, 0b101, 0b111}, // 6
      {0b111, 0b001, 0b001, 0b001, 0b001}, // 7
      {0b111, 0b101, 0b111, 0b101, 0b111}, // 8
      {0b111, 0b101, 0b111, 0b001, 0b111}, // 9
    };

    // Draw each character
    int x_offset = 4;
    int y_offset = 5;
    uint32_t white = SDL_MapRGBA(SDL_GetPixelFormatDetails(fps_surface->format), NULL, 255, 255, 255, 255);

    for (int c = 0; fps_str[c] != '\0'; c++) {
      char ch = fps_str[c];
      if (ch >= '0' && ch <= '9') {
        int digit = ch - '0';
        for (int row = 0; row < 5; row++) {
          for (int col = 0; col < 3; col++) {
            if (digits[digit][row] & (1 << (2 - col))) {
              SDL_Rect pixel = {
                x_offset + col * scale,
                y_offset + row * scale,
                scale,
                scale
              };
              SDL_FillSurfaceRect(fps_surface, &pixel, white);
            }
          }
        }
      }
      x_offset += digit_width + char_spacing;
    }

    // Create texture from surface and render it
    SDL_Texture *fps_texture = SDL_CreateTextureFromSurface(s2d_app.window->sdl_renderer, fps_surface);
    if (fps_texture) {
      SDL_SetTextureBlendMode(fps_texture, SDL_BLENDMODE_BLEND);
      SDL_FRect dst_rect = {5.0f, 5.0f, (float)fps_width, (float)fps_height};
      SDL_RenderTexture(s2d_app.window->sdl_renderer, fps_texture, NULL, &dst_rect);
      SDL_DestroyTexture(fps_texture);
    }
    SDL_DestroySurface(fps_surface);
  }
}


/*
 * Logs an SDL error message and returns a failure result.
 */
SDL_AppResult SDL_Fail() {
  S2D_Error("SDL_Fail", SDL_GetError());
  return SDL_APP_FAILURE;
}


/*
 * Initializes the Simple 2D application, sets up SDL and related libraries,
 * processes command line arguments, retrieves display information, and
 * prepares the main window for rendering.
 */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {

  s2d_app.quit = false;

  // Check command line arguments for diagnostics flag
  S2D_Diagnostics(false);
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--diagnostics") == 0) {
      S2D_Diagnostics(true);
      break;
    }
  }

  S2D_Log(S2D_INFO, "Initializing Simple 2D");

  // Initialize SDL
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
    S2D_Error("SDL_Init", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Initialize SDL_ttf
  if (!TTF_Init()) {
    S2D_Error("TTF_Init", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Initialize SDL_mixer
  if (!MIX_Init()) {
    S2D_Error("MIX_Init", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Create the mixer
  s2d_app.sdl_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
  if (!s2d_app.sdl_mixer) {
    S2D_Error("MIX_CreateMixerDevice", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  // Call the user's `S2D_Init` function
  S2D_Init();

  // Show the window
  if (!S2D_ShowWindow()) return SDL_APP_FAILURE;

  return SDL_APP_CONTINUE;
}


/*
 * Main application iteration function.
 * Handles per-frame logic: updates mouse position, processes held keys,
 * calls user update and render functions, manages renderer state,
 * calculates and displays FPS in diagnostics mode, and presents the frame.
 */
SDL_AppResult SDL_AppIterate(void *appstate) {

  // Get and store the mouse location, mapping from window coordinates
  // back to user logical coordinates by dividing out content_scale
  float mx, my;
  SDL_GetMouseState(&mx, &my);
  float cs = s2d_app.window->display_content_scale;
  s2d_app.window->mouse.x = (int)(mx / cs);
  s2d_app.window->mouse.y = (int)(my / cs);

  // Detect keys held down and call user's `S2D_OnKey` with the event
  int num_keys;
  const bool *key_state = SDL_GetKeyboardState(&num_keys);
  for (int i = 0; i < num_keys; i++) {
    if (key_state[i]) {
      S2D_Event event = {
        .type = S2D_KEY_HELD, .key = SDL_GetScancodeName((SDL_Scancode)i)
      };
      S2D_OnKey(event);
    }
  }

  // Call the user's `S2D_Update` function
  S2D_Update();

  // Set the renderer clear (background) color and clear the screen
  SDL_SetRenderDrawColorFloat(
    s2d_app.window->sdl_renderer,
    s2d_app.window->background.r,
    s2d_app.window->background.g,
    s2d_app.window->background.b,
    s2d_app.window->background.a
  );
  SDL_RenderClear(s2d_app.window->sdl_renderer);

  S2D_Render();  // Call the user's `S2D_Render` function

  // Get actual frame rate and print if in diagnostics mode
  s2d_app.window->fps = get_frame_rate();
  if (s2d_app.diagnostics) {
    draw_frame_rate();
  }

  s2d_app.window->frames++;

  // Flip the buffers to show content rendered on the back buffer
  SDL_RenderPresent(s2d_app.window->sdl_renderer);

  return s2d_app.quit ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}


/*
 * Handles SDL events and dispatches them to the appropriate Simple 2D event handlers
 */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *sdl_event) {

  float mx, my;
  SDL_GetMouseState(&mx, &my);
  float cs = s2d_app.window->display_content_scale;
  mx /= cs;
  my /= cs;

  switch (sdl_event->type) {

    case SDL_EVENT_KEY_DOWN: {
      if (sdl_event->key.repeat == 0) {
        S2D_Event event = {
          .type = S2D_KEY_DOWN, .key = SDL_GetScancodeName(sdl_event->key.scancode)
        };
        S2D_OnKey(event);
      }
      break;
    }

    case SDL_EVENT_KEY_UP: {
      S2D_Event event = {
        .type = S2D_KEY_UP, .key = SDL_GetScancodeName(sdl_event->key.scancode)
      };
      S2D_OnKey(event);
      break;
    }

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
      S2D_Event event = {
        .button = sdl_event->button.button, .x = mx, .y = my
      };
      event.type = sdl_event->type == SDL_EVENT_MOUSE_BUTTON_DOWN ? S2D_MOUSE_DOWN : S2D_MOUSE_UP;
      event.dblclick = sdl_event->button.clicks == 2 ? true : false;
      S2D_OnMouse(event);
      break;
    }

    case SDL_EVENT_MOUSE_WHEEL: {
      S2D_Event event = {
        .type = S2D_MOUSE_SCROLL, .direction = sdl_event->wheel.direction,
        .delta_x = sdl_event->wheel.x, .delta_y = -sdl_event->wheel.y
      };
      S2D_OnMouse(event);
      break;
    }

    case SDL_EVENT_MOUSE_MOTION: {
      S2D_Event event = {
        .type = S2D_MOUSE_MOVE,
        .x = mx, .y = my, .delta_x = (int)(sdl_event->motion.xrel / cs), .delta_y = (int)(sdl_event->motion.yrel / cs)
      };
      S2D_OnMouse(event);
      break;
    }

    case SDL_EVENT_GAMEPAD_ADDED: {
      const SDL_JoystickID id = sdl_event->gdevice.which;
      SDL_Gamepad *pad = SDL_OpenGamepad(id);
      if (!pad) {
        S2D_Error("SDL_OpenGamepad", "SDL_OpenGamepad(%u) failed: %s", (unsigned)id, SDL_GetError());
      } else {
        S2D_Log(S2D_INFO, "Gamepad added: id=%u, name=%s", (unsigned)id, SDL_GetGamepadName(pad));
        S2D_Event event = {
          .type = S2D_CONTROLLER_ADDED, .id = id, .name = SDL_GetGamepadName(pad)
        };
        S2D_OnController(event);
      }
      break;
    }

    case SDL_EVENT_GAMEPAD_REMOVED: {
      const SDL_JoystickID id = sdl_event->gdevice.which;
      SDL_Gamepad *pad = SDL_GetGamepadFromID(id);
      S2D_Log(S2D_INFO, "Gamepad removed: id=%u, name=%s", (unsigned)id, SDL_GetGamepadName(pad));
      S2D_Event event = {
        .type = S2D_CONTROLLER_REMOVED, .id = id, .name = SDL_GetGamepadName(pad)
      };
      S2D_OnController(event);
      SDL_CloseGamepad(pad);
      break;
    }

    case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
      SDL_JoystickID id = sdl_event->gaxis.which;
      S2D_Event event = {
        .type = S2D_AXIS, .id = id,
        .axis = sdl_event->gaxis.axis, .value = sdl_event->gaxis.value
      };
      S2D_OnController(event);
      break;
    }

    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP: {
      SDL_JoystickID id = sdl_event->gbutton.which;
      S2D_Event event = {
        .type = sdl_event->gbutton.down ? S2D_BUTTON_DOWN : S2D_BUTTON_UP, .id = id,
        .button = sdl_event->gbutton.button
      };
      S2D_OnController(event);
      break;
    }

    case SDL_EVENT_WINDOW_RESIZED: {
      // Convert from screen coordinates back to user coordinates
      float cs = s2d_app.window->display_content_scale;
      s2d_app.window->width  = (int)(sdl_event->window.data1 / cs);
      s2d_app.window->height = (int)(sdl_event->window.data2 / cs);
      break;
    }

    case SDL_EVENT_QUIT: {
      S2D_Close();
      break;
    }
  }

  return SDL_APP_CONTINUE;
}


/*
 * Cleans up and shuts down the application, calling the user's `S2D_Quit` function
 */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  S2D_Quit();  // Call the user's `S2D_Quit` function

  // Free library resources
  if (s2d_app.sdl_mixer) MIX_DestroyMixer(s2d_app.sdl_mixer);
  S2D_FreeWindow(s2d_app.window);
  MIX_Quit();
  TTF_Quit();
}
