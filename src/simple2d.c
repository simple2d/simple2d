// simple2d.c

#include "../include/simple2d.h"

// Initalize S2D shared data
char S2D_msg[1024];
bool S2D_diagnostics = false;

// S2D initialization status
static bool initted = false;


/*
 * Checks if a file exists and can be accessed
 */
static bool file_exists(const char *path) {
  if (access(path, F_OK) != -1) {
    return true;
  } else {
    return false;
  }
}


/*
 * Logs standard messages to the console
 */
void S2D_Log(const char *msg, int type) {
  if (S2D_diagnostics) {
    switch (type) {
      case S2D_INFO:
        printf("\033[4;36mInfo:\033[0m %s\n", msg);
        break;
      case S2D_WARN:
        printf("\033[4;33mWarning:\033[0m %s\n", msg);
        break;
      case S2D_ERROR:
        printf("\033[4;31mError:\033[0m %s\n", msg);
        break;
      default:
        printf("%s\n", msg);
    }
  }
}


/*
 * Logs Simple 2D errors to the console, with caller and message body
 */
void S2D_Error(const char *caller, const char *msg) {
  sprintf(S2D_msg, "(%s) %s", caller, msg);
  S2D_Log(S2D_msg, S2D_ERROR);
}


/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status) {
  S2D_diagnostics = status;
}


/*
 * Initialize Simple 2D subsystems
 */
static bool S2D_Init() {
  if (initted) return true;
  
  S2D_Log("Initializing Simple 2D", S2D_INFO);
  
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
 * Quits S2D subsystems
 */
void S2D_Quit() {
  IMG_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  SDL_Quit();
  initted = false;
}


/*
 * Draw a triangle
 */
void S2D_DrawTriangle(GLfloat x1,  GLfloat y1,
                      GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                      GLfloat x2,  GLfloat y2,
                      GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                      GLfloat x3,  GLfloat y3,
                      GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  S2D_GL_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                      x2, y2, c2r, c2g, c2b, c2a,
                      x3, y3, c3r, c3g, c3b, c3a);
}


/*
 * Draw a quad, using two triangles
 */
void S2D_DrawQuad(GLfloat x1,  GLfloat y1,
                  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                  GLfloat x2,  GLfloat y2,
                  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                  GLfloat x3,  GLfloat y3,
                  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a,
                  GLfloat x4,  GLfloat y4,
                  GLfloat c4r, GLfloat c4g, GLfloat c4b, GLfloat c4a) {
  
  S2D_GL_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                      x2, y2, c2r, c2g, c2b, c2a,
                      x3, y3, c3r, c3g, c3b, c3a);
  
  S2D_GL_DrawTriangle(x3, y3, c3r, c3g, c3b, c3a,
                      x4, y4, c4r, c4g, c4b, c4a,
                      x1, y1, c1r, c1g, c1b, c1a);
};


/*
 * Create an image
 * Params: path = image file path
 */
S2D_Image *S2D_CreateImage(const char *path) {
  S2D_Init();
  
  // Check if image file exists
  if (!file_exists(path)) {
    S2D_Error("S2D_CreateImage", "Image file not found");
    return NULL;
  }
  
  // Allocate the image structure
  S2D_Image *img = (S2D_Image *) malloc(sizeof(S2D_Image));
  if (!img) {
    S2D_Error("IMG_Load", "Out of memory!");
    return NULL;
  }
  
  // Load image from file as SDL_Surface
  img->surface = IMG_Load(path);
  if (!img->surface) {
    S2D_Error("IMG_Load", IMG_GetError());
    return NULL;
  }
  
  // Initialize values
  img->x = 0;
  img->y = 0;
  img->color.r = 1.f;
  img->color.g = 1.f;
  img->color.b = 1.f;
  img->color.a = 1.f;
  img->orig_w = img->surface->w;
  img->orig_h = img->surface->h;
  img->w = img->orig_w;
  img->h = img->orig_h;
  img->texture_id = 0;
  
  // Detect image mode
  img->format = GL_RGB;
  if (img->surface->format->BytesPerPixel == 4) {
    img->format = GL_RGBA;
  }
  
  // Flip image bits if BGA
  
  Uint32 r = img->surface->format->Rmask;
  Uint32 g = img->surface->format->Gmask;
  Uint32 a = img->surface->format->Amask;
  
  if (r&0xFF000000 || r&0xFF0000) {
    char *p = (char *)img->surface->pixels;
    int bpp = img->surface->format->BytesPerPixel;
    int w = img->surface->w;
    int h = img->surface->h;
    char tmp;
    for (int i = 0; i < bpp * w * h; i += bpp) {
      if (a&0xFF) {
        tmp = p[i];
        p[i] = p[i+3];
        p[i+3] = tmp;
      }
      if (g&0xFF0000) {
        tmp = p[i+1];
        p[i+1] = p[i+2];
        p[i+2] = tmp;
      }
      if (r&0xFF0000) {
        tmp = p[i];
        p[i] = p[i+2];
        p[i+2] = tmp;
      }
    }
  }
  
  return img;
}


/*
 * Draw an image
 */
void S2D_DrawImage(S2D_Image *img) {
  if (!img) return;
  
  if (img->texture_id == 0) {
    S2D_GL_SetUpTexture(&img->texture_id, img->format,
                        img->orig_w, img->orig_h,
                        img->surface->pixels, GL_NEAREST);
    SDL_FreeSurface(img->surface);
  }
  
  S2D_GL_DrawImage(img);
}


/*
 * Free an image
 */
void S2D_FreeImage(S2D_Image *img) {
  if (!img) return;
  S2D_GL_FreeTexture(&img->texture_id);
  free(img);
}


/*
 * Create a sprite
 * Params: path = image file path
 */
S2D_Sprite *S2D_CreateSprite(const char *path) {
  
  // Check if image file exists
  if (!file_exists(path)) {
    S2D_Error("S2D_CreateSprite", "Sprite image file not found");
    return NULL;
  }
  
  S2D_Sprite *spr = (S2D_Sprite *) malloc(sizeof(S2D_Sprite));
  spr->img = S2D_CreateImage(path);
  
  spr->x = 0;
  spr->y = 0;
  spr->w = spr->img->w;
  spr->h = spr->img->h;
  
  spr->tx1 = 0.f;
  spr->ty1 = 0.f;
  spr->tx2 = 1.f;
  spr->ty2 = 0.f;
  spr->tx3 = 1.f;
  spr->ty3 = 1.f;
  spr->tx4 = 0.f;
  spr->ty4 = 1.f;
  
  return spr;
}


/*
 * Clip a sprite
 */
void S2D_ClipSprite(S2D_Sprite *spr, int x, int y, int w, int h) {
  if (!spr) return;
  
  // Calculate ratios
  // rw = ratio width; rh = ratio height
  double rw = w / (double)spr->img->w;
  double rh = h / (double)spr->img->h;
  
  // Apply ratios to x, y coordinates
  // cx = crop x coord; cy = crop y coord
  double cx = x * rw;
  double cy = y * rh;
  
  // Convert given width, height to doubles
  // cw = crop width; ch = crop height
  double cw = (double)w;
  double ch = (double)h;
  
  // Apply ratio to texture width and height
  // tw = texture width; th = texture height
  double tw = rw * w;
  double th = rh * h;
  
  // Calculate and store sprite texture values
  
  spr->tx1 =  cx       / cw;
  spr->ty1 =  cy       / ch;
  
  spr->tx2 = (cx + tw) / cw;
  spr->ty2 =  cy       / ch;
  
  spr->tx3 = (cx + tw) / cw;
  spr->ty3 = (cy + th) / ch;
  
  spr->tx4 =  cx       / cw;
  spr->ty4 = (cy + th) / ch;
  
  // Store the sprite width and height
  spr->w = w;
  spr->h = h;
}


/*
 * Draw a sprite
 */
void S2D_DrawSprite(S2D_Sprite *spr) {
  if (!spr) return;
  
  if (spr->img->texture_id == 0) {
    S2D_GL_SetUpTexture(&spr->img->texture_id, spr->img->format,
                        spr->img->w, spr->img->h,
                        spr->img->surface->pixels, GL_NEAREST);
    SDL_FreeSurface(spr->img->surface);
  }
  
  S2D_GL_DrawSprite(spr);
}


/*
 * Free a sprite
 */
void S2D_FreeSprite(S2D_Sprite *spr) {
  if (!spr) return;
  S2D_FreeImage(spr->img);
  free(spr);
}


/*
 * Create text
 * Params: font = font file path; msg = text to display; size = font size
 * Returns NULL if text could not be created
 */
S2D_Text *S2D_CreateText(const char *font, const char *msg, int size) {
  S2D_Init();
  
  // Check if font file exists
  if (!file_exists(font)) {
    S2D_Error("S2D_CreateText", "Font file not found");
    return NULL;
  }
  
  // `msg` cannot be an empty string or NULL for TTF_SizeText
  if (msg == NULL || strlen(msg) == 0) msg = " ";
  
  // Allocate the text structure
  S2D_Text *txt = (S2D_Text *) malloc(sizeof(S2D_Text));
  if (!txt) {
    S2D_Error("S2D_CreateText", "Out of memory!");
    return NULL;
  }
  
  // Set default values
  txt->msg = msg;
  txt->x = 0;
  txt->y = 0;
  txt->color.r = 1.0;
  txt->color.g = 1.0;
  txt->color.b = 1.0;
  txt->color.a = 1.0;
  txt->texture_id = 0;
  
  // Open the font
  txt->font = TTF_OpenFont(font, size);
  if (!txt->font) {
    S2D_Error("TTF_OpenFont", TTF_GetError());
    free(txt);
    return NULL;
  }
  
  // Save the width and height of the text
  TTF_SizeText(txt->font, txt->msg, &txt->w, &txt->h);
  
  // Assign color and set up for rendering
  SDL_Color color = { 255, 255, 255 };
  txt->surface = TTF_RenderText_Blended(txt->font, txt->msg, color);
  
  return txt;
}


/*
 * Sets the text message
 */
void S2D_SetText(S2D_Text *txt, const char *msg) {
  if (!txt) return;
  
  // `msg` cannot be an empty string or NULL for TTF_SizeText
  if (msg == NULL || strlen(msg) == 0) msg = " ";
  
  txt->msg = msg;
  
  TTF_SizeText(txt->font, txt->msg, &txt->w, &txt->h);
  
  SDL_Color color = { 255, 255, 255 };
  txt->surface = TTF_RenderText_Blended(txt->font, txt->msg, color);
  
  // Delete the current texture so a new one can be generated
  S2D_GL_FreeTexture(&txt->texture_id);
}


/*
 * Draw text
 */
void S2D_DrawText(S2D_Text *txt) {
  if (!txt) return;
  
  if (txt->texture_id == 0) {
    S2D_GL_SetUpTexture(&txt->texture_id, GL_RGBA,
                        txt->w, txt->h,
                        txt->surface->pixels, GL_NEAREST);
    SDL_FreeSurface(txt->surface);
  }
  
  S2D_GL_DrawText(txt);
}


/*
 * Free the text
 */
void S2D_FreeText(S2D_Text *txt) {
  if (!txt) return;
  S2D_GL_FreeTexture(&txt->texture_id);
  TTF_CloseFont(txt->font);
  free(txt);
}


/*
 * Create a sound
 */
S2D_Sound *S2D_CreateSound(const char *path) {
  S2D_Init();
  
  // Check if sound file exists
  if (!file_exists(path)) {
    S2D_Error("S2D_CreateSound", "Sound file not found");
    return NULL;
  }
  
  // Allocate the sound structure
  S2D_Sound *sound = (S2D_Sound *) malloc(sizeof(S2D_Sound));
  
  // Load the sound data from file
  sound->data = Mix_LoadWAV(path);
  if (!sound->data) {
    S2D_Error("Mix_LoadWAV", Mix_GetError());
    free(sound);
    return NULL;
  }
  
  return sound;
}


/*
 * Play the sound
 */
void S2D_PlaySound(S2D_Sound *sound) {
  if (!sound) return;
  Mix_PlayChannel(-1, sound->data, 0);
}


/*
 * Free the sound
 */
void S2D_FreeSound(S2D_Sound *sound) {
  if (!sound) return;
  Mix_FreeChunk(sound->data);
  free(sound);
}


/*
 * Create the music
 */
S2D_Music *S2D_CreateMusic(const char *path) {
  S2D_Init();
  
  // Check if music file exists
  if (!file_exists(path)) {
    S2D_Error("S2D_CreateMusic", "Music file not found");
    return NULL;
  }
  
  // Allocate the music structure
  S2D_Music *music = (S2D_Music *) malloc(sizeof(S2D_Music));
  if (!music) {
    S2D_Error("S2D_CreateMusic", "Out of memory!");
    return NULL;
  }
  
  // Load the music data from file
  music->data = Mix_LoadMUS(path);
  if (!music->data) {
    S2D_Error("Mix_LoadMUS", Mix_GetError());
    free(music);
    return NULL;
  }
  
  return music;
}


/*
 * Play the music
 */
void S2D_PlayMusic(S2D_Music *music, int times) {
  if (!music) return;
  
  // times: 0 == once, -1 == forever
  if (Mix_PlayMusic(music->data, times) == -1) {
    // No music for you
    S2D_Error("S2D_PlayMusic", Mix_GetError());
  }
}


/*
 * Pause the playing music
 */
void S2D_PauseMusic() {
  Mix_PauseMusic();
}


/*
 * Resume the current music
 */
void S2D_ResumeMusic() {
  Mix_ResumeMusic();
}


/*
 * Stops the playing music; interrupts fader effects
 */
void S2D_StopMusic() {
  Mix_HaltMusic();
}


/*
 * Fade out the playing music
 */
void S2D_FadeOutMusic(int ms) {
  Mix_FadeOutMusic(ms);
}


/*
 * Free the music
 */
void S2D_FreeMusic(S2D_Music *music) {
  if (!music) return;
  Mix_FreeMusic(music->data);
  free(music);
}


/*
 * Create a window
 */
S2D_Window *S2D_CreateWindow(const char *title, int width, int height,
                             S2D_Update update, S2D_Render render, int flags) {
  
  S2D_Init();
  
  // Allocate window and set default values
  S2D_Window *window      = (S2D_Window *) malloc(sizeof(S2D_Window));
  window->sdl             = NULL;
  window->glcontext       = NULL;
  window->title           = title;
  window->width           = width;
  window->height          = height;
  window->viewport.width  = width;
  window->viewport.height = height;
  window->viewport.mode   = S2D_SCALE;
  window->update          = update;
  window->render          = render;
  window->flags           = flags;
  window->on_key          = NULL;
  window->on_key_up       = NULL;
  window->on_key_down     = NULL;
  window->on_mouse        = NULL;
  window->on_controller   = NULL;
  window->vsync           = true;
  window->fps_cap         = 60;
  window->background.r    = 0.0;
  window->background.g    = 0.0;
  window->background.b    = 0.0;
  window->background.a    = 1.0;
  window->close           = true;
  
  // Detect Controllers and Joysticks //////////////////////////////////////////
  
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
          "Balls: %d\n",
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
  
  // Window created by SDL might not actually be the requested size.
  // If not, retrieve and set the actual window size.
  int actual_width, actual_height;
  SDL_GetWindowSize(window->sdl, &actual_width, &actual_height);
  
  if ((window->width != actual_width) || (window->height != actual_height)) {
    sprintf(S2D_msg,
      "Resolution %dx%d unsupported, scaling to %dx%d",
      window->width, window->height, actual_width, actual_height);
    S2D_Log(S2D_msg, S2D_WARN);
    
    window->width  = actual_width;
    window->height = actual_height;
  }
  
  // Set Up OpenGL /////////////////////////////////////////////////////////////
  
  S2D_GL_Init(window);
  
  // Set Main Loop Data ////////////////////////////////////////////////////////
  
  const Uint8 *key_state;
  
  Uint32 frames = 0;           // Total frames since start
  Uint32 start_ms = SDL_GetTicks();  // Elapsed time since start
  Uint32 begin_ms = start_ms;  // Time at beginning of loop
  Uint32 end_ms;               // Time at end of loop
  Uint32 elapsed_ms;           // Total elapsed time
  Uint32 loop_ms;              // Elapsed time of loop
  int delay_ms;                // Amount of delay to achieve desired frame rate
  double fps;                  // The actual frame rate
  
  // Enable VSync
  if (window->vsync) {
    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
      S2D_Log("VSync cannot be enabled", S2D_WARN);
    }
  }
  
  window->close = false;
  
  // Main Loop /////////////////////////////////////////////////////////////////
  
  while (!window->close) {
    
    // Clear Frame /////////////////////////////////////////////////////////////
    
    S2D_GL_Clear(window->background);
    
    // Set FPS /////////////////////////////////////////////////////////////////
    
    frames++;
    end_ms = SDL_GetTicks();
    
    elapsed_ms = end_ms - start_ms;
    fps = frames / (elapsed_ms / 1000.0);
    
    loop_ms = end_ms - begin_ms;
    delay_ms = (1000 / window->fps_cap) - loop_ms;
    
    if (delay_ms < 0) delay_ms = 0;
    
    // Note: `loop_ms + delay_ms` should equal `1000 / fps_cap`
    
    SDL_Delay(delay_ms);
    begin_ms = SDL_GetTicks();
    
    // Handle Input ////////////////////////////////////////////////////////////
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        
        case SDL_KEYDOWN:
          if (window->on_key)
            window->on_key(SDL_GetScancodeName(e.key.keysym.scancode));
          break;
        
        case SDL_KEYUP:
          if (window->on_key_up)
            window->on_key_up(SDL_GetScancodeName(e.key.keysym.scancode));
          break;
        
        case SDL_MOUSEBUTTONDOWN:
          if (window->on_mouse)
            window->on_mouse(e.button.x, e.button.y);
          break;
        
        case SDL_JOYAXISMOTION:
          if (window->on_controller)
            window->on_controller(e.jaxis.which, true, e.jaxis.axis, e.jaxis.value, false, 0);
          break;
        
        case SDL_JOYBUTTONDOWN:
          if (window->on_controller)
            window->on_controller(e.jaxis.which, false, 0, 0, true, e.jbutton.button);
          break;
        
        case SDL_WINDOWEVENT:
          switch (e.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
              // Store new window size
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
      if (window->on_key_down) {
        if (key_state[i] == 1) {
          window->on_key_down(SDL_GetScancodeName(i));
        }
      }
    }
    
    // Store the mouse position
    SDL_GetMouseState(&window->mouse.x, &window->mouse.y);
    
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
    S2D_Log("Closing window", S2D_INFO);
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
