// Simple 2D Shared functions and data

#include "../include/simple2d.h"

// Initalize S2D shared data
bool S2D_diagnostics = false;

// S2D initialization status
static bool initted = false;


/*
 * Provide a `vasprintf()` implementation for Windows
 */
#if WINDOWS && !MINGW
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

  if (access(path, F_OK) != -1) {
    return true;
  } else {
    return false;
  }
}


/*
 * Logs standard messages to the console
 */
void S2D_Log(int type, const char *msg, ...) {

  // Always log if diagnostics set, or if a warning or error message
  if (S2D_diagnostics || type != S2D_INFO) {

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
  S2D_diagnostics = status;
}


/*
 * Enable terminal colors in Windows
 */
void S2D_Windows_EnableTerminalColors() {
  #if WINDOWS
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
  #endif
}


/*
 * Initialize Simple 2D subsystems
 */
bool S2D_Init() {
  if (initted) return true;

  // Enable terminal colors in Windows
  S2D_Windows_EnableTerminalColors();

  S2D_Log(S2D_INFO, "Initializing Simple 2D");

  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    S2D_Error("SDL_Init", SDL_GetError());
    return false;
  }

  // Initialize SDL_ttf
  if (TTF_Init() != 0) {
    S2D_Error("TTF_Init", TTF_GetError());
    return false;
  }

  // Initialize SDL_mixer
  int mix_flags = MIX_INIT_FLAC | MIX_INIT_OGG | MIX_INIT_MP3;
  int mix_initted = Mix_Init(mix_flags);
  if ((mix_initted&mix_flags) != mix_flags) {
    S2D_Error("Mix_Init", Mix_GetError());
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) != 0) {
    S2D_Error("Mix_OpenAudio", Mix_GetError());
    return false;
  }

  // Call `S2D_Quit` at program exit
  atexit(S2D_Quit);

  // All subsystems initted
  initted = true;
  return true;
}


/*
 * Gets the primary display's dimentions
 */
void S2D_GetDisplayDimensions(int *w, int *h) {
  S2D_Init();
  SDL_DisplayMode dm;
  SDL_GetCurrentDisplayMode(0, &dm);
  *w = dm.w;
  *h = dm.h;
}


/*
 * Quits Simple 2D subsystems
 */
void S2D_Quit() {
  IMG_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  SDL_Quit();
  initted = false;
}
