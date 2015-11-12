// OpenGL Shared Functions

#include "../include/simple2d.h"


/*
 * Prints current GL error
 */
void gl_print_error(char *error) {
  asprintf(&s2d_msg, "%s (%d)", error, glGetError());
  S2D_Log(s2d_msg, ERROR);
}


/*
 * Print info about the current OpenGL context
 */
void gl_print_context_info(Window *window) {
  asprintf(&s2d_msg,
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
  S2D_Log(s2d_msg, INFO);
}


/*
 * Store info about the current OpenGL context
 */
void gl_store_context_info(Window *window) {
  window->S2D_GL_VENDOR = glGetString(GL_VENDOR);
  window->S2D_GL_RENDERER = glGetString(GL_RENDERER);
  window->S2D_GL_VERSION = glGetString(GL_VERSION);
  
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
GLuint gl_load_shader(GLenum type, const GLchar *shaderSrc, char *shaderName) {
  
  GLuint shader;
  GLint compiled;
  
  // Create the shader object
  shader = glCreateShader(type);
  
  if (shader == 0) {
    gl_print_error("Failed to create shader program");
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
