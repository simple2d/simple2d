// simple2d.c

#include "../include/simple2d.h"

// Flag to log diagnostic messages for testing
static bool diagnostics = true;

// Flag set if OpenGL 2.1
static bool GL2 = false;

// Set to `true` to force OpenGL 2.1 for testing
static bool FORCE_GL2 = false;


/*
 * Log diagnostic messages
 */
static void diag(char *msg) {
  if (diagnostics) {
    printf("\n\033[4;36mDiagnostics:\033[0m %s\n", msg);
  }
}


/*
 * Print SDL errors and quit
 */
void sdl_error(char *error) {
  printf("%s: %s\n", error, SDL_GetError());
  exit(1);
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
    puts("S2D_DrawImage not yet implemented!");
  #endif
  
  Image img;
  SDL_Surface *surface;
  
  // Load image from file as SDL_Surface
  surface = IMG_Load(path);
  if (!surface) {
    printf("IMG_Load failed: %s\n", IMG_GetError());
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
  //   if (!img.texture) sdl_error("SDL_CreateTextureFromSurface");    
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
    puts("S2D_DrawText not yet implemented!");
  #endif
  
  Text txt;
  
  // `msg` cannot be an empty string; if so, quit
  if (strlen(msg) == 0) {
    puts("S2D_CreateText Error: Text message cannot be empty!");
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
    printf("TTF_OpenFont failed: %s\n", TTF_GetError());
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
  if (!sound.data) {
    printf("Mix_LoadWAV failed: %s\n", Mix_GetError());
  }
  
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
  if (!music.data) {
    printf("Mix_LoadMUS error: %s\n", Mix_GetError());
  }
  
  return music;
}


/*
 * Play the music
 */
void S2D_PlayMusic(Music music, int times) {
  // times: 0 == once, -1 == forever
  if (Mix_PlayMusic(music.data, times) == -1) {
    // No music for you
    printf("\033[4;31mS2D Error:\033[0m S2D_PlayMusic says %s\n", Mix_GetError());
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
  window->background.r = 0.0;
  window->background.g = 0.0;
  window->background.b = 0.0;
  window->background.a = 1.0;
  
  // SDL Initialization ////////////////////////////////////////////////////////
  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) sdl_error("SDL_Init");
  
  // Initialize SDL_ttf
  if (TTF_Init() != 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(1);
  }
  
  // Initialize SDL_mixer
  int mix_flags = MIX_INIT_FLAC|MIX_INIT_OGG|MIX_INIT_MP3;
  int mix_initted = Mix_Init(mix_flags);
  if ((mix_initted&mix_flags) != mix_flags) {
    printf("Mix_Init: Failed to initialize required audio support!\n");
    printf("Mix_Init: %s\n", Mix_GetError());
  }
  
  int audio_rate = 44100;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 4096;
  
  if (Mix_OpenAudio(audio_rate, MIX_DEFAULT_FORMAT, audio_channels, audio_buffers) != 0) {
    printf("Mix_OpenAudio: Unable to initialize audio: %s\n", Mix_GetError());
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
  
  if (!window->sdl) sdl_error("SDL_CreateWindow");
  
  // Window created by SDL might not actually be the requested size
  // If not, retrieve and set the actual window size
  window->s_width = window->width;
  window->s_height = window->height;
  SDL_GetWindowSize(window->sdl, &window->width, &window->height);
  if ((window->width != window->s_width) ||
     (window->height != window->s_height)) {
    printf("Warning: Resolution %dx%d unsupported by driver, scaling to %dx%d\n",
    window->s_width, window->s_height, window->width, window->height);
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
      sdl_error("GLES / SDL_GL_CreateContext");
    
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
        sdl_error("GL2 / SDL_GL_CreateContext");
      }
    #endif
  }
  
  gl_store_context_info(window);
  gl_print_context_info(window);
  
  return window;
}


/*
 * Show the window
 */
int S2D_Show(Window *window) {
  
  // Setting up variables
  int cursor_x, cursor_y;  // Cursor positions
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
      printf("Warning: VSync cannot be enabled");
    }
  }
  
  // Detect Controllers and Joysticks //////////////////////////////////////////
  
  printf("Number of Joysticks: %i\n", SDL_NumJoysticks());
  
  // Variables for controllers and joysticks
  SDL_GameController *controller = NULL;
  SDL_Joystick *joy = NULL;
  
  // Enumerate joysticks
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    
    // Check to see if joystick supports SDL's game controller interface
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
      if (controller) {
        printf("Found a valid controller, named: %s\n", SDL_GameControllerName(controller));
        break;  // Break after first available controller
      } else {
        fprintf(stderr, "Could not open game controller %i: %s\n", i, SDL_GetError());
      }
    
    // Controller interface not supported, try to open as joystick
    } else {
      printf("Joystick %i is not supported by the game controller interface!\n", i);
      joy = SDL_JoystickOpen(i);
      
      // Joystick is valid
      if (joy) {
        printf("Opened Joystick %i\n", i);
        printf("Name: %s\n", SDL_JoystickName(joy));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
      
      // Joystick not valid
      } else {
        printf("Couldn't open Joystick %i\n", i);
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
          // TODO: Register the mouse click, add callback
          printf("Mouse down at: %i, %i\n", e.button.x, e.button.y);
          break;
        case SDL_CONTROLLERBUTTONDOWN:
          puts("SDL_CONTROLLERBUTTONDOWN");
          break;
        case SDL_JOYAXISMOTION:
          printf("SDL_JOYAXISMOTION: axis=%i, value=%i\n", e.jaxis.axis, e.jaxis.value);
          break;
        case SDL_JOYBUTTONDOWN:
          printf("SDL_JOYBUTTONDOWN: %i\n", e.jbutton.button);
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
    
    // Store the cursor position
    SDL_GetMouseState(&cursor_x, &cursor_y);
    
    // Store new values in the window
    window->cursor.x      = cursor_x;
    window->cursor.y      = cursor_y;
    window->cursor.real_x = cursor_x;
    window->cursor.real_y = cursor_y;
    window->frames        = frames;
    window->elapsed_ms    = elapsed_ms;
    window->loop_ms       = loop_ms;
    window->delay_ms      = delay_ms;
    window->fps           = fps;
    
    // scale the cursor position, if necessary
    if (window->s_width != window->width) {
      window->cursor.x = (int)((double)window->cursor.real_x *
        ((double)window->s_width / (double)window->width) + 0.5);
    }
    
    if (window->s_height != window->height) {
      window->cursor.y = (int)((double)window->cursor.real_y *
        ((double)window->s_height / (double)window->height) + 0.5);
    }
    
    // Call update and render callbacks
    if (window->update) window->update();
    if (window->render) window->render();
    
    // Draw Frame //////////////////////////////////////////////////////////////
    SDL_GL_SwapWindow(window->sdl);
  }
  
  // Quitting clean up
  IMG_Quit();
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  SDL_GL_DeleteContext(window->glcontext);
  SDL_DestroyWindow(window->sdl);
  SDL_Quit();
  
  return 0;
}
