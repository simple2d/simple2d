// OpenGL ES 2.0

#include "../include/simple2d.h"

static GLuint programObject;
static GLuint colorLocation;


/*
 * Testing
 */
void hello_gles() {
  puts("Hello from OpenGL ES!");
}


/*
 * Initalize OpenGL ES
 */
int init_gles(int width, int height, int s_width, int s_height) {
  
  print_gl_context();
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Set the viewport
  glViewport(0, 0, width, height);
  
  // Vertex shader source string
  GLchar vShaderStr[] =
    // uniforms used by the vertex shader
    "uniform mat4 u_mvpMatrix;"  // model view and projection matrix
    
    // attributes input to the vertex shader
    "attribute vec4 a_position;"  // position value
    "attribute vec4 a_color;"     // input vertex color
    
    // varying variables, input to the fragment shader
    "varying vec4 v_color;"  // output vertex color
    
    "void main()"
    "{"
    "  vec4 position = vec4(a_position.xyz, 1.0);"
    "  gl_Position = u_mvpMatrix * position;"
    "  v_color = a_color;"
    "}";
  
  // Fragment shader source string
  GLchar fShaderStr[] =
    "precision mediump float;"
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    "void main()"
    "{"
    "  gl_FragColor = v_color;"
    "}";
  
  // Shader variables
  GLuint vertexShader;
  GLuint fragmentShader;
  GLint linked;
  
  // Load the vertex and fragment shaders
  vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
  fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);
  
  // Create and store the program object
  programObject = glCreateProgram();
  
  if (programObject == 0) {
    printf("Failed to create program object: %d\n", glGetError());
    return 1;
  }
  
  glAttachShader(programObject, vertexShader);
  glAttachShader(programObject, fragmentShader);
  
  // Bind vPosition to attribute 0
  glBindAttribLocation(programObject, 0, "a_position");
  
  // Link the program
  glLinkProgram(programObject);
  
  // Check the link status
  glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
  
  if (!linked) {
    
    GLint infoLen = 0;
    glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
    
    if (infoLen > 1) {
      
      char* infoLog = malloc (sizeof(char) * infoLen);
      
      glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
      printf("Error linking program:\n%s\n", infoLog);
      
      free(infoLog);
    }
    
    glDeleteProgram(programObject);
    return GL_FALSE;
  }
  
  // Use the program object
  glUseProgram(programObject);
  
  // Compute scaling factors, if necessary
  GLfloat scale_x = 1.0f;
  GLfloat scale_y = 1.0f;
  
  if (s_width != width) {
    scale_x = (GLfloat)s_width / (GLfloat)width;
  }
  
  if (s_height != height) {
    scale_y = (GLfloat)s_height / (GLfloat)height;
  }
  
  // Create and apply an orthographic projection matrix (2D projection)
  // (matrix is given in column-first order)
  int far_z = 128;
  GLfloat orthoMatrix[16] = {
    2.0f / ((GLfloat)width * scale_x), 0, 0, 0,
    0, -2.0f / ((GLfloat)height * scale_y), 0, 0,
    0, 0, -2.0f / (GLfloat)far_z, 0,
    -1.0f, 1.0f, -1.0f, 1.0f
  };
  
  GLuint mMvpLocation = glGetUniformLocation(programObject, "u_mvpMatrix");
  glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  return GL_TRUE;
}


/*
 * Draw triangle
 */
void draw_triangle_gles(GLfloat x1,  GLfloat y1,
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
  
  // Set the color
  colorLocation = glGetAttribLocation(programObject, "a_color");
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
void draw_image_gles(Image img, int x, int y) {
  // TODO: Implement this
}


/*
 * Draw text
 */
void draw_text_gles(Text txt, int x, int y) {
  // TODO: Implement this
}
