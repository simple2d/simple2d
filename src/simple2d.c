// simple2d.c

#include "../include/simple2d.h"

// Flag for printing diagnostic messages
static bool diagnostics = false;

// Flag set if OpenGL 2.1
static bool GL2 = false;

// Set to `true` to force OpenGL 2.1 for testing
static bool FORCE_GL2 = false;



/*
 * Logs standard messages to the console
 */
void S2D_Log(char *msg, int type) {
  
  if (diagnostics) {
    switch (type) {
      case INFO:
        printf("\033[4;36mInfo:\033[0m %s\n", msg);
        break;
      case WARN:
        printf("\033[4;33mWarning:\033[0m %s\n", msg);
        break;
      case ERROR:
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
void S2D_Error(char *caller, const char *msg) {
  asprintf(&s2d_msg, "(%s) %s", caller, msg);
  S2D_Log(s2d_msg, ERROR);
}


/*
 * Enable/disable logging of diagnostics
 */
void S2D_Diagnostics(bool status) {
  diagnostics = status;
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
  
  #if GLES
    gles_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
  #else
    if (GL2) {
      gl2_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                        x2, y2, c2r, c2g, c2b, c2a,
                        x3, y3, c3r, c3g, c3b, c3a);
    } else {
      gl3_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                        x2, y2, c2r, c2g, c2b, c2a,
                        x3, y3, c3r, c3g, c3b, c3a);
    }
  #endif
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
  
  #if GLES
    gles_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
    
    gles_draw_triangle(x3, y3, c3r, c3g, c3b, c3a,
                       x4, y4, c4r, c4g, c4b, c4a,
                       x1, y1, c1r, c1g, c1b, c1a);
  #else
    if (GL2) {
      gl2_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                        x2, y2, c2r, c2g, c2b, c2a,
                        x3, y3, c3r, c3g, c3b, c3a);
      
      gl2_draw_triangle(x3, y3, c3r, c3g, c3b, c3a,
                        x4, y4, c4r, c4g, c4b, c4a,
                        x1, y1, c1r, c1g, c1b, c1a);      
    } else {
      gl3_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                        x2, y2, c2r, c2g, c2b, c2a,
                        x3, y3, c3r, c3g, c3b, c3a);
      
      gl3_draw_triangle(x3, y3, c3r, c3g, c3b, c3a,
                        x4, y4, c4r, c4g, c4b, c4a,
                        x1, y1, c1r, c1g, c1b, c1a);
    }
  #endif
};


/*
 * Create an image
 */
Image S2D_CreateImage(char *path) {
  
  // TODO: Implement images in GLES
  #if GLES
    S2D_Log("S2D_DrawImage not yet implemented!", INFO);
  #endif
  
  Image img;
  SDL_Surface *surface;
  
  // Load image from file as SDL_Surface
  surface = IMG_Load(path);
  if (!surface) {
    S2D_Error("IMG_Load", IMG_GetError());
    exit(1);
  }
  
  img.x = 0;
  img.y = 0;
  
  // Save the width and height of the surface
  img.w = surface->w;
  img.h = surface->h;
  
  // TESTING ///////////////////////////////////////////////////////////////////
  // SDL_RWops *rwop;
  // rwop=SDL_RWFromFile(path, "rb");
  // if(IMG_isBMP(rwop))
  // printf("sample.bmp is a BMP file.\n");
  // else
  // printf("sample.bmp is not a BMP file, or BMP support is not available.\n");
  // 
  // if (GL2) {
  //   img.texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  //   if (!img.texture) S2D_Error("SDL_CreateTextureFromSurface", SDL_GetError());
  // } else {
  // }
  //////////////////////////////////////////////////////////////////////////////
  
  img.texture_id = 0;
  
  // Generate texture names
  glGenTextures(1, &img.texture_id);
  
  // Bind the named texture to a texturing target
  glBindTexture(GL_TEXTURE_2D, img.texture_id);
  
  // TODO: BMP is in BGR, I think
  // Detect image mode
  int mode = GL_RGB;
  if(surface->format->BytesPerPixel == 4) {
    mode = GL_RGBA;
  }
  
  // TESTING ///////////////////////////////////////////////////////////////////
  // printf("\n%s\n", path);
  // printf("texture_id: %i\n", img.texture_id);
  // if (mode == GL_RGB) puts("Mode is GL_RGB");
  // if (mode == GL_RGBA) puts("Mode is GL_RGBA");
  // printf("mode: %i\n\n", mode);
  //////////////////////////////////////////////////////////////////////////////
  
  // Specifies the 2D texture image
  glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);
  
  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Free the surface data, no longer needed
  SDL_FreeSurface(surface);
  
  return img;
}


/*
 * Draw an image
 */
void S2D_DrawImage(Image img) {
  #if GLES
    gles_draw_image(img);
  #else
    if (GL2) {
      gl2_draw_image(img);
    } else {
      gl3_draw_image(img);
    }
  #endif
}


/*
 * Free an image
 */
void S2D_FreeImage(Image img) {
  glDeleteTextures(1, &img.texture_id);
}


/*
 * Create text
 */
Text S2D_CreateText(char *font, char *msg, int size) {
  
  #if GLES
    S2D_Log("S2D_DrawText not yet implemented!", WARN);
  #endif
  
  Text txt;
  
  // `msg` cannot be an empty string; if so, quit
  if (strlen(msg) == 0) {
    S2D_Error("S2D_CreateText", "Text message cannot be empty!");
    exit(1);
  } else {
    txt.msg = msg;
  }
  
  txt.x = 0;
  txt.y = 0;
  txt.color.r = 1.0;
  txt.color.g = 1.0;
  txt.color.b = 1.0;
  txt.color.a = 1.0;
  
  txt.font = TTF_OpenFont(font, size);
  if (!txt.font) {
    S2D_Error("TTF_OpenFont", TTF_GetError());
    exit(1);
  }
  
  // Save the width and height of the text
  TTF_SizeText(txt.font, txt.msg, &txt.w, &txt.h);
  
  SDL_Surface *surface;
  SDL_Color color = { 255, 255, 255 };
  surface = TTF_RenderText_Blended(txt.font, txt.msg, color);
  
  txt.texture_id = 0;
  
  // Generate texture names
  glGenTextures(1, &txt.texture_id);
  
  // Bind the named texture to a texturing target
  glBindTexture(GL_TEXTURE_2D, txt.texture_id);
  
  // Specifies the 2D texture image
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txt.w, txt.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  
  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Free the surface data, no longer needed
  SDL_FreeSurface(surface);
  
  return txt;
}


/*
 * Sets the text message
 */
void S2D_SetText(Text *txt, char *msg) {
  
  txt->msg = msg;
  
  TTF_SizeText(txt->font, txt->msg, &txt->w, &txt->h);
  
  SDL_Surface *surface;
  SDL_Color color = { 255, 255, 255 };
  surface = TTF_RenderText_Blended(txt->font, txt->msg, color);
  
  glBindTexture(GL_TEXTURE_2D, txt->texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txt->w, txt->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  SDL_FreeSurface(surface);
}


/*
 * Draw text
 */
void S2D_DrawText(Text txt) {
  
  #if GLES
    gles_draw_text(txt);
  #else
    if (GL2) {
      gl2_draw_text(txt);
    } else {
      gl3_draw_text(txt);
    }
  #endif
}


/*
 * Free the text
 */
void S2D_FreeText(Text txt) {
  glDeleteTextures(1, &txt.texture_id);
  TTF_CloseFont(txt.font);
}


/*
 * Create a sound
 */
Sound S2D_CreateSound(char *path) {
  Sound sound;
  
  sound.data = Mix_LoadWAV(path);
  if (!sound.data) S2D_Error("Mix_LoadWAV", Mix_GetError());
  
  return sound;
}


/*
 * Play the sound
 */
void S2D_PlaySound(Sound sound) {
  Mix_PlayChannel(-1, sound.data, 0);
}


/*
 * Free the sound
 */
void S2D_FreeSound(Sound sound) {
  Mix_FreeChunk(sound.data);
}


/*
 * Create the music
 */
Music S2D_CreateMusic(char *path) {
  Music music;
  
  music.data = Mix_LoadMUS(path);
  if (!music.data) S2D_Error("Mix_LoadMUS", Mix_GetError());
  
  return music;
}


/*
 * Play the music
 */
void S2D_PlayMusic(Music music, int times) {
  // times: 0 == once, -1 == forever
  if (Mix_PlayMusic(music.data, times) == -1) {
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
void S2D_FreeMusic(Music music) {
  Mix_FreeMusic(music.data);
}


/*
 * Create a window
 */
Window* S2D_CreateWindow(char* title, int width, int height,
                         Update update, Render render) {
  
  // Allocate window and set default values
  Window *window = (Window*)malloc(sizeof(Window));
  window->title = title;
  window->width = width;
  window->height = height;
  window->fps_cap = 60;
  window->vsync = true;
  window->update = update;
  window->render = render;
  window->on_key = NULL;
  window->on_key_down = NULL;
  window->on_mouse = NULL;
  window->background.r = 0.0;
  window->background.g = 0.0;
  window->background.b = 0.0;
  window->background.a = 1.0;
  
  // SDL Initialization ////////////////////////////////////////////////////////
  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) S2D_Error("SDL_Init", SDL_GetError());
  
  // Initialize SDL_ttf
  if (TTF_Init() != 0) {
    S2D_Error("TTF_Init", TTF_GetError());
    exit(1);
  }
  
  // Initialize SDL_mixer
  int mix_flags = MIX_INIT_FLAC|MIX_INIT_OGG|MIX_INIT_MP3;
  int mix_initted = Mix_Init(mix_flags);
  if ((mix_initted&mix_flags) != mix_flags) {
    S2D_Error("Mix_Init", Mix_GetError());
  }
  
  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  
  if (Mix_OpenAudio(audio_rate, MIX_DEFAULT_FORMAT, audio_channels, audio_buffers) != 0) {
    S2D_Error("Mix_OpenAudio", Mix_GetError());
    exit(1);
  }
  
  // Create SDL window
  // TODO: Add `SDL_WINDOW_FULLSCREEN_DESKTOP` option to flags, or...
  //       Call `SDL_SetWindowFullscreen` in update loop
  // TODO: Add SDL_WINDOW_ALLOW_HIGHDPI flag
  window->sdl = SDL_CreateWindow(
    window->title,                                   // title
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  // window position
    window->width, window->height,                   // window size
    SDL_WINDOW_OPENGL                                // flags
  );
  
  if (!window->sdl) S2D_Error("SDL_CreateWindow", SDL_GetError());
  
  // Window created by SDL might not actually be the requested size.
  // If not, retrieve and set the actual window size.
  window->s_width = window->width;
  window->s_height = window->height;
  SDL_GetWindowSize(window->sdl, &window->width, &window->height);
  
  if ((window->width != window->s_width) ||
    (window->height != window->s_height)) {
    
    asprintf(&s2d_msg,
      "Resolution %dx%d unsupported by driver, scaling to %dx%d",
      window->s_width, window->s_height, window->width, window->height);
    S2D_Log(s2d_msg, WARN);
  }
  
  // Init OpenGL / GLES ////////////////////////////////////////////////////////
  
  #if !GLES
    if (FORCE_GL2) {
      // Use legacy OpenGL 2.1
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    } else {
      // Request an OpenGL 3.3 forward-compatible core profile
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    }
  #endif
  
  if (FORCE_GL2) {
    window->glcontext = NULL;
  } else {
    // Ask SDL to create an OpenGL context
    window->glcontext = SDL_GL_CreateContext(window->sdl);
  }
  
  // Check for a valid OpenGL context
  if (window->glcontext) {
    
    // Initialize OpenGL ES 2.0
    #if GLES
      gles_init(window->width, window->height, window->s_width, window->s_height);
    
    // Initialize OpenGL 3.3 or forward-compatible core profile
    #else
      gl3_init(window->width, window->height);
    #endif
  
  // Could not create context
  } else {
    
    // Fail on OpenGL ES
    #if GLES
      S2D_Error("GLES / SDL_GL_CreateContext", SDL_GetError());
      
    // Try to fallback on an OpenGL 2.1 context
    #else
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
      
      // Check for a valid OpenGL context
      window->glcontext = SDL_GL_CreateContext(window->sdl);
      if (window->glcontext) {
        GL2 = true;
        gl2_init(window->width, window->height);
        
        // Could not create any GL contexts
      } else {
        S2D_Error("GL2 / SDL_GL_CreateContext", SDL_GetError());
      }
    #endif
  }
  
  gl_store_context_info(window);
  if (diagnostics) gl_print_context_info(window);
  
  return window;
}


/*
 * Show the window
 */
int S2D_Show(Window *window) {
  
  // Setting up variables
  int mouse_x, mouse_y;  // Mouse positions
  const Uint8 *key_state;
  Uint32 frames = 0;       // Total frames since start
  Uint32 start_ms = SDL_GetTicks();  // Elapsed time since start
  Uint32 begin_ms = start_ms;  // Time at beginning of loop
  Uint32 end_ms;    // Time at end of loop
  Uint32 elapsed_ms;  // Total elapsed time
  Uint32 loop_ms;   // Elapsed time of loop
  int delay_ms;     // Amount of delay to achieve desired frame rate
  double fps;       // The actual frame rate
  
  // Enable VSync
  if (window->vsync) {
    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
      S2D_Log("VSync cannot be enabled", WARN);
    }
  }
  
  // Detect Controllers and Joysticks //////////////////////////////////////////
  
  if (SDL_NumJoysticks() > 0) {
    asprintf(&s2d_msg, "Joysticks detected: %i", SDL_NumJoysticks());
    S2D_Log(s2d_msg, INFO);
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
        asprintf(&s2d_msg, "Found a valid controller, named: %s\n",
                 SDL_GameControllerName(controller));
        S2D_Log(s2d_msg, INFO);
        break;  // Break after first available controller
      } else {
        asprintf(&s2d_msg, "Could not open game controller %i: %s\n", i, SDL_GetError());
        S2D_Log(s2d_msg, ERROR);
      }
    
    // Controller interface not supported, try to open as joystick
    } else {
      asprintf(&s2d_msg, "Joystick %i is not supported by the game controller interface", i);
      S2D_Log(s2d_msg, WARN);
      joy = SDL_JoystickOpen(i);
      
      // Joystick is valid
      if (joy) {
        asprintf(&s2d_msg,
          "Opened Joystick %i\n"
          "Name: %s\n"
          "Axes: %d\n"
          "Buttons: %d\n"
          "Balls: %d\n",
          i, SDL_JoystickName(joy), SDL_JoystickNumAxes(joy),
          SDL_JoystickNumButtons(joy), SDL_JoystickNumBalls(joy)
        );
        S2D_Log(s2d_msg, INFO);
        
      // Joystick not valid
      } else {
        asprintf(&s2d_msg, "Could not open Joystick %i", i);
        S2D_Log(s2d_msg, ERROR);
      }
      
      break;  // Break after first available joystick
    }
  }
  
  // Main Event Loop ///////////////////////////////////////////////////////////
  
  bool quit = false;
  while (!quit) {
    
    // Clear Frame /////////////////////////////////////////////////////////////
    
    glClearColor(
      window->background.r,
      window->background.g,
      window->background.b,
      window->background.a
    );
    glClear(GL_COLOR_BUFFER_BIT);
    
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
          if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            quit = true;
          }
          if (window->on_key) {
            window->on_key(SDL_GetScancodeName(e.key.keysym.scancode));
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          if (window->on_mouse)
            window->on_mouse(e.button.x, e.button.y);
          break;
          
        case SDL_JOYAXISMOTION:
          break;
        case SDL_JOYBUTTONDOWN:
          break;
          
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
    
    int num_keys;
    key_state = SDL_GetKeyboardState(&num_keys);
    
    for (int i = 0; i < num_keys; i++) {
      if (window->on_key_down) {
        if (key_state[i] == 1) {
          window->on_key_down(SDL_GetScancodeName(i));
        }
      }
    }
    
    // Update Window State /////////////////////////////////////////////////////
    
    // Store the mouse position
    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    // Store new values in the window
    window->mouse.x      = mouse_x;
    window->mouse.y      = mouse_y;
    window->mouse.real_x = mouse_x;
    window->mouse.real_y = mouse_y;
    window->frames       = frames;
    window->elapsed_ms   = elapsed_ms;
    window->loop_ms      = loop_ms;
    window->delay_ms     = delay_ms;
    window->fps          = fps;
    
    // scale the mouse position, if necessary
    if (window->s_width != window->width) {
      window->mouse.x = (int)((double)window->mouse.real_x *
        ((double)window->s_width / (double)window->width) + 0.5);
    }
    
    if (window->s_height != window->height) {
      window->mouse.y = (int)((double)window->mouse.real_y *
        ((double)window->s_height / (double)window->height) + 0.5);
    }
    
    // Call update and render callbacks
    if (window->update) window->update();
    if (window->render) window->render();
    
    // Draw Frame //////////////////////////////////////////////////////////////
    SDL_GL_SwapWindow(window->sdl);
  }
  
  return 0;
}


/*
 * Close the window, clean up SDL
 */
int S2D_Close(Window *window) {
  
  S2D_Log("Closing S2D", INFO);
  
  // S2D
  free(s2d_msg);
  
  // SDL
  IMG_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  SDL_GL_DeleteContext(window->glcontext);
  SDL_DestroyWindow(window->sdl);
  SDL_Quit();
  
  return 0;
}
