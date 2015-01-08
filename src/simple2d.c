// simple2d.c

#include "../include/simple2d.h"

static bool GL2 = false;
static bool GL3 = false;


/*
 * Print SDL errors
 */
void sdl_error(char *error) {
  printf("%s: %s\n", error, SDL_GetError());
  exit(1);
}


/*
 * Print info about the current OpenGL context
 */
void print_gl_context() {
  printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
  printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
  printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
  printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
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
    draw_triangle_gles(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
  #else
    if (GL2) {
      draw_triangle_gl2(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
    } else {
      draw_triangle_gl3(x1, y1, c1r, c1g, c1b, c1a,
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
    draw_triangle_gles(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
    
    draw_triangle_gles(x3, y3, c3r, c3g, c3b, c3a,
                       x4, y4, c4r, c4g, c4b, c4a,
                       x1, y1, c1r, c1g, c1b, c1a);
  #else
    
    if (GL2) {
      draw_triangle_gl2(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
      
      draw_triangle_gl2(x3, y3, c3r, c3g, c3b, c3a,
                       x4, y4, c4r, c4g, c4b, c4a,
                       x1, y1, c1r, c1g, c1b, c1a);      
    } else {
      draw_triangle_gl3(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
      
      draw_triangle_gl3(x3, y3, c3r, c3g, c3b, c3a,
                       x4, y4, c4r, c4g, c4b, c4a,
                       x1, y1, c1r, c1g, c1b, c1a);
    }
  #endif
};


/*
 * Create an image
 */
Image S2D_CreateImage(Window *window, char *path) {
  
  #if GLES
    puts("S2D_DrawImage not yet implemented!");
  #endif
  
  Image img;
  SDL_Surface *surface;
  
  surface = IMG_Load(path);
  if (!surface) {
    printf("IMG_Load failed: %s\n", IMG_GetError());
    exit(1);
  }
  
  img.texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  if (!img.texture) sdl_error("SDL_CreateTextureFromSurface");
  
  SDL_FreeSurface(surface);
  
  return img;
}


/*
 * Draw an image
 */
void S2D_DrawImage(Image img) {
  #if GLES
    draw_image_gles(img);
  #else
    if (GL2) {
      draw_image_gl2(img);
    } else {
      draw_image_gl3(img);
    }
  #endif
}


/*
 * Free an image
 */
void S2D_FreeImage(Image img) {
  SDL_DestroyTexture(img.texture);
}


/*
 * Create text
 */
Text S2D_CreateText(Window *window, char *font, char *msg, int size) {
  
  #if GLES
    puts("S2D_DrawText not yet implemented!");
  #endif
  
  Text txt;
  
  txt.font = TTF_OpenFont(font, size);
  if(!txt.font) {
    printf("TTF_OpenFont failed: %s\n", TTF_GetError());
    exit(1);
  }
  
  SDL_Color color = { 255, 255, 255 };
  txt.color.r = 1.0;
  txt.color.g = 1.0;
  txt.color.b = 1.0;
  txt.color.a = 1.0;
  
  txt.msg = msg;
  SDL_Surface *surface;
  surface = TTF_RenderText_Blended(txt.font, txt.msg, color);
  txt.texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  
  // Calculate the width and height of the text
  TTF_SizeText(txt.font, txt.msg, &txt.w, &txt.h);
  
  SDL_FreeSurface(surface);
  
  return txt;
}


/*
* Sets the text message
*/
void S2D_SetText(Window *window, Text *txt, char *msg) {
  SDL_DestroyTexture(txt->texture);
  
  SDL_Color color = { 255, 255, 255 };
  txt->msg = msg;
  SDL_Surface *surface;
  surface = TTF_RenderText_Blended(txt->font, txt->msg, color);
  txt->texture = SDL_CreateTextureFromSurface(window->renderer, surface);
  TTF_SizeText(txt->font, txt->msg, &txt->w, &txt->h);
  
  SDL_FreeSurface(surface);
}


/*
 * Draw text
 */
void S2D_DrawText(Text txt) {
  #if GLES
    draw_text_gles(txt);
  #else
    if (GL2) {
      draw_text_gl2(txt);
    } else {
      draw_text_gl3(txt);
    }
  #endif
}


/*
 * Free the text
 */
void S2D_FreeText(Text txt) {
  SDL_DestroyTexture(txt.texture);
  TTF_CloseFont(txt.font);  
}


/*
 * Create a sound
 */
Sound S2D_CreateSound(char *path) {
  Sound sound;
  
  sound.wave = Mix_LoadWAV(path);
  if (!sound.wave) {
    printf("Mix_LoadWAV failed: %s\n", Mix_GetError());
  }
  
  return sound;
}


/*
 * Play the sound
 */
void S2D_PlaySound(Sound sound) {
  Mix_PlayChannel(-1, sound.wave, 0);
}


/*
 * Free the sound
 */
void S2D_FreeSound(Sound sound) {
  Mix_FreeChunk(sound.wave);
}


/*
 * Create a window
 */
Window* S2D_CreateWindow(char* title, int width, int height,
                         Update update, Render render,
                         On_key on_key, Key_down key_down) {
  
  Window *window = (Window*)malloc(sizeof(Window));
  window->title = title;
  window->width = width;
  window->height = height;
  window->fps_cap = 60;
  window->vsync = true;
  window->update = update;
  window->render = render;
  window->on_key = on_key;
  window->key_down = key_down;
  window->background.r = 0.0;
  window->background.g = 0.0;
  window->background.b = 0.0;
  window->background.a = 1.0;
  window->renderer = NULL;
  
  // SDL inits
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  TTF_Init();
  
  
  // Create SDL window
  // TODO: Add `SDL_WINDOW_FULLSCREEN_DESKTOP` option to flags, or...
  //       Call `SDL_SetWindowFullscreen` in update loop
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
  
  #if GLES
    GLES = true;
  #else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  #endif
  
  // OpenGL inits
  window->glcontext = SDL_GL_CreateContext(window->sdl);
  
  if (window->glcontext) {
    #if !GLES
      GL3 = true;
    #endif
  } else {
    // Try creating OpenGL 2.1 context fallback
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
    window->glcontext = SDL_GL_CreateContext(window->sdl);
    if (window->glcontext) {
      GL2 = true;
    } else {
      // Could not create a GL context
      sdl_error("SDL_GL_CreateContext");
    }
  }
  
  #if GLES
    init_gles(window->width, window->height);
  #else
    if (GL2) {
      init_gl2(window->width, window->height);
      
      // Create 2D rendering context for converting SDL_Surface to SDL_Texture,
      // using the `SDL_CreateTextureFromSurface` function
      window->renderer = SDL_CreateRenderer(window->sdl, -1,
                                            SDL_RENDERER_TARGETTEXTURE);
      if (!window->renderer) sdl_error("SDL_CreateRenderer");
    } else {
      init_gl3(window->width, window->height);
    }
  #endif
  
  // Use OpenGL context instead of SDL_Renderer
  SDL_GL_MakeCurrent(window->sdl, window->glcontext);
  
  // TODO: Remove when done testing
  #if GLES
    hello_gles();
  #else
    if (GL2) {
      hello_gl2();
    } else {
      hello_gl3();
    }
  #endif
  
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
  
  
  printf("Number of Joysticks: %i\n", SDL_NumJoysticks());
  
  SDL_GameController *controller = NULL;
  SDL_Joystick *joy = NULL;
  
  for (int i = 0; i < SDL_NumJoysticks(); ++i) {
    if (SDL_IsGameController(i)) {
      controller = SDL_GameControllerOpen(i);
      if (controller) {
        printf("Found a valid controller, named: %s\n", SDL_GameControllerName(controller));
        break;
      } else {
        fprintf(stderr, "Could not open game controller %i: %s\n", i, SDL_GetError());
      }
    } else {
      printf("Joystick %i is not supported by the game controller interface!\n", i);
      joy = SDL_JoystickOpen(i);
      if (joy) {
        printf("Opened Joystick 0\n");
        printf("Name: %s\n", SDL_JoystickName(joy));
        printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
        printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
      } else {
        printf("Couldn't open Joystick %i\n", i);
      }
      break;
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
    
    // Note: loop_ms + delay_ms => should equal (1000 / fps_cap)
    
    SDL_Delay(delay_ms);
    begin_ms = SDL_GetTicks();
    
    // Handle Input ////////////////////////////////////////////////////////////
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_KEYDOWN:
          if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            quit = true;
          }
          if (window->on_key) {
            window->on_key(SDL_GetScancodeName(e.key.keysym.scancode));
          }
          break;
        case SDL_MOUSEBUTTONDOWN:
          // TODO: Register the mouse click
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
      if (window->key_down) {
        if (key_state[i] == 1) {
          window->key_down(SDL_GetScancodeName(i));
        }
      }
    }
    
    // Update Window State /////////////////////////////////////////////////////
    
    // Store the cursor position
    SDL_GetMouseState(&cursor_x, &cursor_y);
    
    // Store new values in the window
    window->cursor_x   = cursor_x;
    window->cursor_y   = cursor_y;
    window->a_cursor_x = cursor_x;
    window->a_cursor_y = cursor_y;
    window->frames     = frames;
    window->elapsed_ms = elapsed_ms;
    window->loop_ms    = loop_ms;
    window->delay_ms   = delay_ms;
    window->fps        = fps;
    
    // scale the cursor position, if necessary
    if (window->s_width != window->width) {
      window->cursor_x = (int)((double)window->a_cursor_x *
        ((double)window->s_width / (double)window->width) + 0.5);
    }
    
    if (window->s_height != window->height) {
      window->cursor_y = (int)((double)window->a_cursor_y *
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
  Mix_Quit();
  SDL_GL_DeleteContext(window->glcontext);
  if (window->renderer) SDL_DestroyRenderer(window->renderer);
  SDL_DestroyWindow(window->sdl);
  SDL_Quit();
  
  return 0;
}
