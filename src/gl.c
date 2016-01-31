// Simple 2D OpenGL Functions

#include "../include/simple2d.h"

// The orthographic projection matrix for 2D rendering,
// given in column-first order.
GLfloat S2D_GL_orthoMatrix[16] = {
   2.0f,     0,             0,    0,
      0, -2.0f,             0,    0,
      0,     0, -2.0f / 128.0,    0,  // 128.0 == far_z
  -1.0f,  1.0f,         -1.0f, 1.0f
};


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
void S2D_GL_PrintContextInfo(Window *window) {
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
void S2D_GL_StoreContextInfo(Window *window) {
  window->S2D_GL_VENDOR   = glGetString(GL_VENDOR);
  window->S2D_GL_RENDERER = glGetString(GL_RENDERER);
  window->S2D_GL_VERSION  = glGetString(GL_VERSION);
  
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
      
      char* infoLog = malloc(sizeof(char)*infoLen);
      
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
 * Sets the view and matrix projection
 */
void S2D_GL_SetView(int window_width,       int window_height,
                    int s2d_viewport_width, int s2d_viewport_height) {
  
  #if GLES
    // gles_set_view(window_width, window_height, s2d_viewport_width, s2d_viewport_height);
  #else
    if (S2D_GL2) {
      // gl2_set_view(window_width, window_height, s2d_viewport_width, s2d_viewport_height);
    } else {
      gl3_set_view(window_width, window_height, s2d_viewport_width, s2d_viewport_height);
    }
  #endif
}


/*
 * Prepares a texture for rendering.
 */
void S2D_GL_SetUpTexture(GLuint *id, GLint format, int w, int h, const GLvoid *data, GLint filter) {
  
  // If 0, then a new texture; generate name
  if (*id == 0) {
    glGenTextures(1, id);
  }
  
  // Bind the named texture to a texturing target
  glBindTexture(GL_TEXTURE_2D, *id);
  
  // Specifies the 2D texture image
  glTexImage2D(
    GL_TEXTURE_2D, 0, format, w, h,
    0, format, GL_UNSIGNED_BYTE, data
  );
  
  // Set texture parameters
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
    gles_draw_triangle(x1, y1, c1r, c1g, c1b, c1a,
                       x2, y2, c2r, c2g, c2b, c2a,
                       x3, y3, c3r, c3g, c3b, c3a);
  #else
    if (S2D_GL2) {
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
 * Draw an image
 */
void S2D_GL_DrawImage(Image *img) {
  #if GLES
    gles_draw_image(img);
  #else
    if (S2D_GL2) {
      gl2_draw_image(img);
    } else {
      gl3_draw_image(img);
    }
  #endif
}


/*
 * Draw text
 */
void S2D_GL_DrawText(Text *txt) {
  #if GLES
    gles_draw_text(txt);
  #else
    if (S2D_GL2) {
      gl2_draw_text(txt);
    } else {
      gl3_draw_text(txt);
    }
  #endif
}


/*
 * Free a texture
 */
void S2D_GL_FreeTexture(GLuint *id) {
  glDeleteTextures(1, id);
}


/*
 * Clear buffers to given color values
 */
void S2D_GL_Clear(Color clr) {
  glClearColor(clr.r, clr.g, clr.b, clr.a);
  glClear(GL_COLOR_BUFFER_BIT);
}
