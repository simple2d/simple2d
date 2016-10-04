// window.c

#include "../include/simple2d.h"


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
  
  // Detect controllers and joysticks
  S2D_DetectControllers();
  
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
    
    sprintf(S2D_msg, "Scaling window to %ix%i (requested size was %ix%i)",
      actual_width, actual_height, window->width, window->height);
    S2D_Log(S2D_msg, S2D_INFO);
    
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
    
    int mx, my;  // mouse x, y coordinates
    
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
          if (window->on_mouse) {
            S2D_GetMouseOnViewport(window, e.button.x, e.button.y, &mx, &my);
            window->on_mouse(mx, my);
          }
          break;
        
        case SDL_JOYAXISMOTION:
          if (window->on_controller)
            window->on_controller(
              e.jaxis.which, true, e.jaxis.axis, e.jaxis.value, false, 0
            );
          break;
        
        case SDL_JOYBUTTONDOWN:
          if (window->on_controller)
            window->on_controller(
              e.jaxis.which, false, 0, 0, true, e.jbutton.button
            );
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
      if (window->on_key_down) {
        if (key_state[i] == 1) {
          window->on_key_down(SDL_GetScancodeName(i));
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
