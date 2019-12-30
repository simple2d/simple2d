// Simple 2D OpenGL Functions

#include "../include/simple2d.h"

// Set to `true` to force OpenGL 2.1 (for testing)
static bool FORCE_GL2 = false;

// Flag set if using OpenGL 2.1
static bool S2D_GL2 = false;

// The orthographic projection matrix for 2D rendering.
// Elements 0 and 5 are set in S2D_GL_SetViewport.
static GLfloat orthoMatrix[16] =
  {    0,    0,     0,    0,
       0,    0,     0,    0,
       0,    0,     0,    0,
   -1.0f, 1.0f, -1.0f, 1.0f };


/*
 * Prints current GL error
 */
void S2D_GL_PrintError(char *error) {
  S2D_Log(S2D_ERROR, "%s (%d)", error, glGetError());
}


/*
 * Print info about the current OpenGL context
 */
void S2D_GL_PrintContextInfo(S2D_Window *window) {
  S2D_Log(S2D_INFO,
    "OpenGL Context\n"
    "      GL_VENDOR: %s\n"
    "      GL_RENDERER: %s\n"
    "      GL_VERSION: %s\n"
    "      GL_SHADING_LANGUAGE_VERSION: %s",
    window->S2D_GL_VENDOR,
    window->S2D_GL_RENDERER,
    window->S2D_GL_VERSION,
    window->S2D_GL_SHADING_LANGUAGE_VERSION
  );
}


/*
 * Store info about the current OpenGL context
 */
void S2D_GL_StoreContextInfo(S2D_Window *window) {

  window->S2D_GL_VENDOR   = glGetString(GL_VENDOR);
  window->S2D_GL_RENDERER = glGetString(GL_RENDERER);
  window->S2D_GL_VERSION  = glGetString(GL_VERSION);

  // These are not defined in GLES
  #if GLES
    window->S2D_GL_MAJOR_VERSION = 0;
    window->S2D_GL_MINOR_VERSION = 0;
  #else
    glGetIntegerv(GL_MAJOR_VERSION, &window->S2D_GL_MAJOR_VERSION);
    glGetIntegerv(GL_MINOR_VERSION, &window->S2D_GL_MINOR_VERSION);
  #endif

  window->S2D_GL_SHADING_LANGUAGE_VERSION = glGetString(GL_SHADING_LANGUAGE_VERSION);
};


/*
 * Creates a shader object, loads shader string, and compiles.
 * Returns 0 if shader could not be compiled.
 */
GLuint S2D_GL_LoadShader(GLenum type, const GLchar *shaderSrc, char *shaderName) {

  // Create the shader object
  GLuint shader = glCreateShader(type);

  if (shader == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return 0;
  }

  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);

  // Compile the shader
  glCompileShader(shader);

  // Check the compile status
  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLint infoLen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = malloc(sizeof(char) * infoLen);
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      printf("Error compiling shader \"%s\":\n%s\n", shaderName, infoLog);
      free(infoLog);
    }

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}


/*
 * Check if shader program was linked
 */
int S2D_GL_CheckLinked(GLuint program, char *name) {

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);

  if (!linked) {
    GLint infoLen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1) {
      char *infoLog = malloc(sizeof(char) * infoLen);
      glGetProgramInfoLog(program, infoLen, NULL, infoLog);
      printf("Error linking program `%s`: %s\n", name, infoLog);
      free(infoLog);
    }

    glDeleteProgram(program);
    return GL_FALSE;
  }

  return GL_TRUE;
}


/*
 * Calculate the viewport's scaled width and height
 */
void S2D_GL_GetViewportScale(S2D_Window *window, int *w, int *h, double *scale) {

  double s = fmin(
    window->width  / (double)window->viewport.width,
    window->height / (double)window->viewport.height
  );

  *w = window->viewport.width  * s;
  *h = window->viewport.height * s;

  if (scale) *scale = s;
}


/*
 * Sets the viewport and matrix projection
 */
void S2D_GL_SetViewport(S2D_Window *window) {

  int ortho_w = window->viewport.width;
  int ortho_h = window->viewport.height;
  int x, y, w, h;  // calculated GL viewport values

  x = 0; y = 0; w = window->width; h = window->height;

  switch (window->viewport.mode) {

    case S2D_FIXED:
      w = window->orig_width;
      h = window->orig_height;
      y = window->height - h;
      break;

    case S2D_EXPAND:
      ortho_w = w;
      ortho_h = h;
      break;

    case S2D_SCALE:
      S2D_GL_GetViewportScale(window, &w, &h, NULL);
      // Center the viewport
      x = window->width  / 2.0 - w/2.0;
      y = window->height / 2.0 - h/2.0;
      break;

    case S2D_STRETCH:
      break;
  }

  glViewport(x, y, w, h);

  // Set orthographic projection matrix
  orthoMatrix[0] =  2.0f / (GLfloat)ortho_w;
  orthoMatrix[5] = -2.0f / (GLfloat)ortho_h;

  #if GLES
    S2D_GLES_ApplyProjection(orthoMatrix);
  #else
    if (S2D_GL2) {
      S2D_GL2_ApplyProjection(ortho_w, ortho_h);
    } else {
      S2D_GL3_ApplyProjection(orthoMatrix);
    }
  #endif
}


/*
 * Initialize OpenGL
 */
int S2D_GL_Init(S2D_Window *window) {

  // Specify OpenGL contexts and set attributes
  #if GLES
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  #else
    // Use legacy OpenGL 2.1
    if (FORCE_GL2) {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Request an OpenGL 3.3 forward-compatible core profile
    } else {
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    }
  #endif

  // Create and store the OpenGL context
  if (FORCE_GL2) {
    window->glcontext = NULL;
  } else {
    // Ask SDL to create an OpenGL context
    window->glcontext = SDL_GL_CreateContext(window->sdl);
  }

  // Check if a valid OpenGL context was created
  if (window->glcontext) {
    // Valid context found

    // Initialize OpenGL ES 2.0
    #if GLES
      S2D_GLES_Init();
      S2D_GL_SetViewport(window);

    // Initialize OpenGL 3.3+
    #else
      // Initialize GLEW on Windows
      #if WINDOWS
        GLenum err = glewInit();
        if (GLEW_OK != err) S2D_Error("GLEW", glewGetErrorString(err));
      #endif
      S2D_GL3_Init();
      S2D_GL_SetViewport(window);
    #endif

  // Context could not be created
  } else {

    #if GLES
      S2D_Error("GLES / SDL_GL_CreateContext", SDL_GetError());

    #else
      // Try to fallback using an OpenGL 2.1 context
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

      // Try creating the context again
      window->glcontext = SDL_GL_CreateContext(window->sdl);

      // Check if this context was created
      if (window->glcontext) {
        // Valid context found
        S2D_GL2 = true;
        S2D_GL2_Init();
        S2D_GL_SetViewport(window);

      // Could not create any OpenGL contexts, hard failure
      } else {
        S2D_Error("GL2 / SDL_GL_CreateContext", SDL_GetError());
        S2D_Log(S2D_ERROR, "An OpenGL context could not be created");
        return -1;
      }
    #endif
  }

  // Store the context and print it if diagnostics is enabled
  S2D_GL_StoreContextInfo(window);
  if (S2D_diagnostics) S2D_GL_PrintContextInfo(window);

  return 0;
}


/*
 * Creates a texture for rendering
 */
void S2D_GL_CreateTexture(GLuint *id, GLint format,
                          int w, int h,
                          const GLvoid *data, GLint filter) {

  // If 0, then a new texture; generate name
  if (*id == 0) glGenTextures(1, id);

  // Bind the named texture to a texturing target
  glBindTexture(GL_TEXTURE_2D, *id);

  // Specifies the 2D texture image
  glTexImage2D(
    GL_TEXTURE_2D, 0, format, w, h,
    0, format, GL_UNSIGNED_BYTE, data
  );

  // Set the filtering mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}


/*
 * Free a texture
 */
void S2D_GL_FreeTexture(GLuint *id) {
  if (*id != 0) {
    glDeleteTextures(1, id);
    *id = 0;
  }
}


/*
 * Draw a triangle
 */
void S2D_GL_DrawTriangle(GLfloat x1, GLfloat y1,
                         GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                         GLfloat x2, GLfloat y2,
                         GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                         GLfloat x3, GLfloat y3,
                         GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  #if GLES
    S2D_GLES_DrawTriangle(x1, y1, r1, g1, b1, a1,
                          x2, y2, r2, g2, b2, a2,
                          x3, y3, r3, g3, b3, a3);
  #else
    if (S2D_GL2) {
      S2D_GL2_DrawTriangle(x1, y1, r1, g1, b1, a1,
                           x2, y2, r2, g2, b2, a2,
                           x3, y3, r3, g3, b3, a3);
    } else {
      S2D_GL3_DrawTriangle(x1, y1, r1, g1, b1, a1,
                           x2, y2, r2, g2, b2, a2,
                           x3, y3, r3, g3, b3, a3);
    }
  #endif
}


/*
 * Draw an image
 */
void S2D_GL_DrawImage(S2D_Image *img) {
  #if GLES
    S2D_GLES_DrawImage(img);
  #else
    if (S2D_GL2) {
      S2D_GL2_DrawImage(img);
    } else {
      S2D_GL3_DrawImage(img);
    }
  #endif
}


/*
 * Draw sprite
 */
void S2D_GL_DrawSprite(S2D_Sprite *spr) {
  #if GLES
    S2D_GLES_DrawSprite(spr);
  #else
    if (S2D_GL2) {
      S2D_GL2_DrawSprite(spr);
    } else {
      S2D_GL3_DrawSprite(spr);
    }
  #endif
}


/*
 * Draw text
 */
void S2D_GL_DrawText(S2D_Text *txt) {
  #if GLES
    S2D_GLES_DrawText(txt);
  #else
    if (S2D_GL2) {
      S2D_GL2_DrawText(txt);
    } else {
      S2D_GL3_DrawText(txt);
    }
  #endif
}


/*
 * Clear buffers to given color values
 */
void S2D_GL_Clear(S2D_Color clr) {
  glClearColor(clr.r, clr.g, clr.b, clr.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
