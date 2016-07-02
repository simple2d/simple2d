// Simple 2D OpenGL Functions

#include "../include/simple2d.h"

// Set to `true` to force OpenGL 2.1 (for testing)
static bool FORCE_GL2 = false;

// Flag set if using OpenGL 2.1
static bool S2D_GL2 = false;

// The orthographic projection matrix for 2D rendering,
// given in column-first order.
static GLfloat orthoMatrix[16] =
  { 2.0f,     0,             0,    0,
       0, -2.0f,             0,    0,
       0,     0, -2.0f / 128.0,    0,  // 128.0 == far_z
   -1.0f,  1.0f,         -1.0f, 1.0f };


/*
 * Prints current GL error
 */
void S2D_GL_PrintError(char *error) {
  sprintf(S2D_msg, "%s (%d)", error, glGetError());
  S2D_Log(S2D_msg, S2D_ERROR);
}


/*
 * Print info about the current OpenGL context
 */
void S2D_GL_PrintContextInfo(S2D_Window *window) {
  sprintf(S2D_msg,
    "OpenGL Context\n"
    "GL_VENDOR: %s\n"
    "GL_RENDERER: %s\n"
    "GL_VERSION: %s\n"
    "GL_MAJOR_VERSION: %i\n"
    "GL_MINOR_VERSION: %i\n"
    "GL_SHADING_LANGUAGE_VERSION: %s",
    window->S2D_GL_VENDOR,
    window->S2D_GL_RENDERER,
    window->S2D_GL_VERSION,
    window->S2D_GL_MAJOR_VERSION,
    window->S2D_GL_MINOR_VERSION,
    window->S2D_GL_SHADING_LANGUAGE_VERSION
  );
  S2D_Log(S2D_msg, S2D_INFO);
}


/*
 * Store info about the current OpenGL context
 */
void S2D_GL_StoreContextInfo(S2D_Window *window) {
  window->S2D_GL_VENDOR   = glGetString(GL_VENDOR);
  window->S2D_GL_RENDERER = glGetString(GL_RENDERER);
  window->S2D_GL_VERSION  = glGetString(GL_VERSION);
  
  // These are not defined in GLES
  #if !GLES
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
  
  GLuint shader;
  GLint compiled;
  
  // Create the shader object
  shader = glCreateShader(type);
  
  if (shader == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return 0;
  }
  
  // Load the shader source
  glShaderSource(shader, 1, &shaderSrc, NULL);
  
  // Compile the shader
  glCompileShader(shader);
  
  // Check the compile status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  
  if (!compiled) {
    
    GLint infoLen = 0;
    
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    
    if (infoLen > 1) {
      
      char *infoLog = malloc(sizeof(char) * infoLen);
      
      glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
      printf("Error compiling shader %s:\n%s\n", shaderName, infoLog);
      
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
 * Sets the viewport and matrix projection
 */
void S2D_GL_SetViewport(S2D_Window *window) {
  
  int ortho_w = window->viewport.width;
  int ortho_h = window->viewport.height;
  int x, y, w, h;  // Calculated GL viewport values
  double scale;
  
  x = 0; y = 0; w = window->width; h = window->height;
  
  switch (window->viewport.mode) {
    
    case S2D_FIXED:
      ortho_w = window->width;
      ortho_h = window->height;
      break;
      
    case S2D_SCALE:
      scale = fmin(
        window->width  / (double)window->viewport.width,
        window->height / (double)window->viewport.height
      );
      
      w = window->viewport.width  * scale;
      h = window->viewport.height * scale;
      
      // center the viewport
      x = window->width  / 2 - w/2;
      y = window->height / 2 - h/2;
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
 * Init OpenGL
 */
int S2D_GL_Init(S2D_Window *window) {
  
  // Specify the OpenGL Context
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
    
    #if GLES
      // Initialize OpenGL ES 2.0
      S2D_GLES_Init();
      S2D_GL_SetViewport(window);
      
    #else
      // Initialize OpenGL 3.3+
      S2D_GL3_Init();
      S2D_GL_SetViewport(window);
    #endif
    
  } else {
    // Context could not be created
    
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
        
      } else {
        // Could not create any OpenGL contexts, hard failure
        S2D_Error("GL2 / SDL_GL_CreateContext", SDL_GetError());
        S2D_Log("An OpenGL context could not be created", S2D_ERROR);
        free(window);
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
 * Prepares a texture for rendering
 */
void S2D_GL_SetUpTexture(GLuint *id, GLint format,
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
 * Draw a triangle
 */
void S2D_GL_DrawTriangle(GLfloat x1,  GLfloat y1,
                         GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                         GLfloat x2,  GLfloat y2,
                         GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                         GLfloat x3,  GLfloat y3,
                         GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  #if GLES
    S2D_GLES_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                          x2, y2, c2r, c2g, c2b, c2a,
                          x3, y3, c3r, c3g, c3b, c3a);
  #else
    if (S2D_GL2) {
      S2D_GL2_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                           x2, y2, c2r, c2g, c2b, c2a,
                           x3, y3, c3r, c3g, c3b, c3a);
    } else {
      S2D_GL3_DrawTriangle(x1, y1, c1r, c1g, c1b, c1a,
                           x2, y2, c2r, c2g, c2b, c2a,
                           x3, y3, c3r, c3g, c3b, c3a);
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
 * Free a texture
 */
void S2D_GL_FreeTexture(GLuint *id) {
  if (*id != 0) {
    glDeleteTextures(1, id);
    *id = 0;
  }
}


/*
 * Clear buffers to given color values
 */
void S2D_GL_Clear(S2D_Color clr) {
  glClearColor(clr.r, clr.g, clr.b, clr.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
