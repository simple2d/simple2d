// simple2d.c
#include "../include/simple2d.h"


/*
 * Print SDL errors
 */
static void error(char *error) {
  printf("%s failed: %s\n", error, SDL_GetError());
  exit(1);
}


/*
 * Init window for OpenGL
 */
bool initGL(width, height) {
  GLenum error = GL_NO_ERROR;
  
  glViewport(0, 0, width, height);
  glOrtho(0, width, height, 0, -1.f, 1.f);
  
  // Initialize the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  
  // Initialize the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Check for errors
  error = glGetError();
  if (error != GL_NO_ERROR) {
    printf( "OpenGL initialization failed! Error code: %i\n", error);
    return false;
  } else {
    return true;
  }
}


/*
 * Draw an OpenGL triangle
 */
void S2D_DrawTriangle(GLfloat x1,  GLfloat y1,
                      GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                      GLfloat x2,  GLfloat y2,
                      GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                      GLfloat x3,  GLfloat y3,
                      GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  glBegin(GL_TRIANGLES);
    
    glColor4f(c1r, c1g, c1b, c1a);
    glVertex2f(x1, y1);
    
    glColor4f(c2r, c2g, c2b, c2a);
    glVertex2f(x2, y2);
    
    glColor4f(c3r, c3g, c3b, c3a);
    glVertex2f(x3, y3);
    
  glEnd();
}


/*
 * Draw a quad, using two OpenGL triangles
 */
void S2D_DrawQuad(GLfloat x1,  GLfloat y1,
                  GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                  GLfloat x2,  GLfloat y2,
                  GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                  GLfloat x3,  GLfloat y3,
                  GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a,
                  GLfloat x4,  GLfloat y4,
                  GLfloat c4r, GLfloat c4g, GLfloat c4b, GLfloat c4a) {
  
  S2D_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                   x2, y2, c2r, c2g, c2b, c2a,
                   x3, y3, c3r, c3g, c3b, c3a);
  S2D_DrawTriangle(x3, y3, c3r, c3g, c3b, c3a,
                   x4, y4, c4r, c4g, c4b, c4a,
                   x1, y1, c1r, c1g, c1b, c1a);
};


/*
 * Create an image
 */
Image* S2D_CreateImage(Window *window, char *path) {
  Image *img = (Image*)malloc(sizeof(Image));
  
  img->surface = IMG_Load(path);
  if(!img->surface) {
    printf("IMG_Load failed: %s\n", IMG_GetError());
    exit(1);
  }
  
  img->texture = SDL_CreateTextureFromSurface(window->renderer, img->surface);
  if (!img->texture) { error("SDL_CreateTextureFromSurface"); }
  
  return img;
}


/*
 * Draw an image in OpenGL
 */
void S2D_DrawImage(Image *img, int x, int y) {
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  if (SDL_GL_BindTexture(img->texture, NULL, NULL)) {
    puts("Simple2D Warning: image cannot be drawn");
  }
  
  int w = img->surface->w;
  int h = img->surface->h;
  
  glBegin(GL_QUADS);
    
    glColor4f(1, 1, 1, 1);
    
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    
    glTexCoord2f(w, 0);
    glVertex2f(x + w, y);
    
    glTexCoord2f(w, h);
    glVertex2f(x + w, y + h);
    
    glTexCoord2f(0, h);
    glVertex2f(x, y + h);
    
  glEnd();
  
  SDL_GL_UnbindTexture(img->texture);
}


/*
 * Free an image
 */
void S2D_FreeImage(Image *img) {
  SDL_DestroyTexture(img->texture);
  SDL_FreeSurface(img->surface);
}


/*
 * Create a window
 */
Window* S2D_CreateWindow(char* title, int width, int height, int fps_cap, bool vsync,
                        Update update, On_key on_key, Key_down key_down) {
  
  Window *window = (Window*)malloc(sizeof(Window));
  window->title = title;
  window->width = width;
  window->height = height;
  window->fps_cap = fps_cap;
  window->vsync = vsync;
  window->update = update;
  window->on_key = on_key;
  window->key_down = key_down;
  
  // SDL inits
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  TTF_Init();
  
  // Create SDL window
  window->sdl_window = SDL_CreateWindow(
    window->title,                                   // title
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,  // window position
    window->width, window->height,                   // window size
    SDL_WINDOW_OPENGL                                // flags
  );
  
  if (!window->sdl_window) { error("SDL_CreateWindow"); }
  
  // Enable VSync
  if (window->vsync) {
    if (!SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1")) {
      printf("Simple2D Warning: VSync cannot be enabled!");
    }
  }
  
  // OpenGL inits
  window->glcontext = SDL_GL_CreateContext(window->sdl_window);
  if (!window->glcontext) { error("SDL_GL_CreateContext"); }
  initGL(window->width, window->height);
  
  // Create SDL renderer for accelerated 2D
  window->renderer = SDL_CreateRenderer(window->sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (!window->renderer) { error("SDL_CreateRenderer"); }
  
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
  Uint32 begin_ms = start_ms;  // TIme at beginning of loop
  Uint32 end_ms;    // Time at end of loop
  Uint32 total_ms;  // Total elapsed time
  Uint32 loop_ms;   // Elapsed time of loop
  int delay_ms;     // Amount of delay to achieve desired frame rate
  double fps;       // The actual frame rate
  
  // Main Event Loop ///////////////////////////////////////////////////////////
  
  bool quit = false;
  while (!quit) {
    
    // Set FPS /////////////////////////////////////////////////////////////////
    
    frames++;
    end_ms = SDL_GetTicks();
    
    total_ms = end_ms - start_ms;
    fps = frames / (total_ms / 1000.0);
    
    loop_ms = end_ms - begin_ms;
    delay_ms = (1000 / window->fps_cap) - loop_ms;
    
    if (delay_ms < 0) { delay_ms = 0; }
    
    // Note: loop_ms + delay_ms => should equal (1000 / fps_cap)
    
    SDL_Delay(delay_ms);
    begin_ms = SDL_GetTicks();
    
    // Handle Input ////////////////////////////////////////////////////////////
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch(e.type) {
        case SDL_KEYDOWN:
          window->on_key(SDL_GetScancodeName(e.key.keysym.scancode));
          break;
        case SDL_QUIT:
          quit = true;
          break;
      }
    }
    
    int num_keys;
    key_state = SDL_GetKeyboardState(&num_keys);
    
    for (int i = 0; i < num_keys; i++) {
      if (key_state[i] == 1) {
        window->key_down(SDL_GetScancodeName(i));
      }
    }
    
    // Update Application State ////////////////////////////////////////////////
    
    // Store the cursor position
    SDL_GetMouseState(&cursor_x, &cursor_y);
    
    window->cursor_x = cursor_x;
    window->cursor_y = cursor_y;
    window->frames = frames;
    window->total_ms = total_ms;
    window->loop_ms = loop_ms;
    window->delay_ms = delay_ms;
    window->fps = fps;
    
    // Clear frame
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Call update callback
    window->update();
    
    // Draw frame
    SDL_GL_SwapWindow(window->sdl_window);
  }
  
  // Clean up
  IMG_Quit();
  Mix_Quit();
  SDL_GL_DeleteContext(window->glcontext);
  SDL_DestroyRenderer(window->renderer);
  SDL_DestroyWindow(window->sdl_window);
  SDL_Quit();
  
  return 0;
}
