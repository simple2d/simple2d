// OpenGL ES 2.0

#include "../include/simple2d.h"

static GLuint shaderProgram;
static GLuint texShaderProgram;
static GLuint colorLocation;

// For GL3 and GLES, an orthographic 2D projection matrix.
// Matrix is given in column-first order.
static GLfloat orthoMatrix[16] = {
  2.0f, 0, 0, 0,
  0, -2.0f, 0, 0,
  0, 0, -2.0f / 128.0, 0,  // 128.0 == far_z
  -1.0f, 1.0f, -1.0f, 1.0f
};


/*
 * Check if shader program was linked
 */
int gles_check_linked(GLuint program, char *name) {
  GLint linked;
  
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  
  if (!linked) {
    
    GLint infoLen = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
    
    if (infoLen > 1) {
      
      char* infoLog = malloc (sizeof(char) * infoLen);
      
      glGetProgramInfoLog(program, infoLen, NULL, infoLog);
      printf("Error linking program `%s`: %s\n", name, infoLog);
      
      free(infoLog);
    }
    
    glDeleteProgram(program);
    return GL_FALSE;
  }
}


/*
 * Initalize OpenGL ES
 */
int gles_init(int width, int height, int s_width, int s_height) {
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Set the viewport
  glViewport(0, 0, width, height);
  
  // Vertex shader source string
  GLchar vertexSource[] =
    // uniforms used by the vertex shader
    "uniform mat4 u_mvpMatrix;"  // model view and projection matrix
    
    // attributes input to the vertex shader
    "attribute vec4 a_position;"  // position value
    "attribute vec4 a_color;"     // input vertex color
    "attribute vec2 a_texcoord;"  // input texture
    
    // varying variables, input to the fragment shader
    "varying vec4 v_color;"     // output vertex color
    "varying vec2 v_texcoord;"  // output texture
    
    // main
    "void main()"
    "{"
    "  v_color = a_color;"
    "  v_texcoord = a_texcoord;"
    "  gl_Position = u_mvpMatrix * vec4(a_position.xyz, 1.0);"
    "}";
  
  // Fragment shader source string
  GLchar fragmentSource[] =
    "precision mediump float;"
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    "void main()"
    "{"
    "  gl_FragColor = v_color;"
    "}";
  
  // Fragment shader source string for textures
  GLchar texFragmentSource[] =
    "precision mediump float;"
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    "varying vec2 v_texcoord;"
    "uniform sampler2D texture;"
    "void main()"
    "{"
    "  gl_FragColor = texture(texture, v_texcoord) * v_color;"
    "}";
  
  // Load the vertex and fragment shaders
  GLuint vertexShader      = S2D_GL_LoadShader(  GL_VERTEX_SHADER,      vertexSource, "GLES Vertex");
  GLuint fragmentShader    = S2D_GL_LoadShader(GL_FRAGMENT_SHADER,    fragmentSource, "GLES Fragment");
  GLuint texFragmentShader = S2D_GL_LoadShader(GL_FRAGMENT_SHADER, texFragmentSource, "GLES Texture Fragment");
  
  // Create the shader program object
  shaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (shaderProgram == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }
  
  // Create the texture shader program object
  texShaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (texShaderProgram == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }
  
  // Attach the shader objects to the program object
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);
  
  // Associate user-defined attribute variables in the
  // program object with the vertex attribute index
  glBindAttribLocation(shaderProgram, 0, "a_position");
  glBindAttribLocation(texShaderProgram, 0, "a_position");
  
  // Link the shader program
  glLinkProgram(shaderProgram);
  glLinkProgram(texShaderProgram);
  
  // Check if linked
  gles_check_linked(shaderProgram, "shaderProgram");
  gles_check_linked(texShaderProgram, "texShaderProgram");
  
  // Compute scaling factors, if necessary
  GLfloat scale_x = 1.0f;
  GLfloat scale_y = 1.0f;
  
  if (s_width != width) {
    scale_x = (GLfloat)s_width / (GLfloat)width;
  }
  
  if (s_height != height) {
    scale_y = (GLfloat)s_height / (GLfloat)height;
  }
  
  // Set orthographic projection matrix
  orthoMatrix[0] = 2.0f / ((GLfloat)width * scale_x);
  orthoMatrix[5] = -2.0f / ((GLfloat)height * scale_y);
  
  // Use the program object
  glUseProgram(shaderProgram);
  
  GLuint mMvpLocation = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  // Use the texture program object
  glUseProgram(texShaderProgram);
  
  GLuint texmMvpLocation = glGetUniformLocation(texShaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(texmMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);
  
  return GL_TRUE;
}


/*
 * Draw triangle
 */
void gles_draw_triangle(GLfloat x1,  GLfloat y1,
                        GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                        GLfloat x2,  GLfloat y2,
                        GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                        GLfloat x3,  GLfloat y3,
                        GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  GLfloat vVertices[] =
    { x1, y1, 0.0f,
      x2, y2, 0.0f,
      x3, y3, 0.0f };
  
  GLfloat vColors[] =
    { c1r, c1g, c1b, c1a,
      c2r, c2g, c2b, c2a,
      c3r, c3g, c3b, c3a };
  
  glUseProgram(shaderProgram);
  
  // Set the color
  colorLocation = glGetAttribLocation(shaderProgram, "a_color");
  glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, vColors);
  glEnableVertexAttribArray(colorLocation);
  
  // Load the vertex data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
  glEnableVertexAttribArray(0);
  
  // draw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


/*
 * Draw image
 */
void gles_draw_image(Image img) {
  // TODO: Implement this
}


/*
 * Draw text
 */
void gles_draw_text(Text txt) {
  // TODO: Implement this
}
