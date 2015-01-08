// OpenGL 3.3

#include "../include/simple2d.h"


/*
 * Testing
 */
void hello_gl3() {
  puts("Hello from OpenGL 3.3!");
}


/*
 * Initalize OpenGL
 */
int init_gl3(int width, int height) {
  
  print_gl_context();
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Vertex shader source string
  const GLchar* vertexSource =
    "#version 150 core\n"
    "uniform mat4 u_mvpMatrix;"
    "in vec2 position;"
    "in vec4 color;"
    "out vec4 Color;"
    "void main() {"
    "  Color = color;"
    "  gl_Position = u_mvpMatrix * vec4(position, 0.0, 1.0);"
    "}";
  
  // Fragment shader source string
  const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec4 Color;"
    "out vec4 outColor;"
    "void main() {"
    // "  outColor = vec4(Color, 1.0);"
    "  outColor = Color;"
    "}";
  
  // Create Vertex Array Object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Create a Vertex Buffer Object
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  // Create and compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  
  // Create and compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  
  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);
  
  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
  
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  
  
  // Create and apply an orthographic projection matrix (2D projection)
  // (matrix is given in column-first order)
  int far_z = 128;
  GLfloat orthoMatrix[16] = {
    2.0f / (GLfloat)width, 0, 0, 0,
    0, -2.0f / (GLfloat)height, 0, 0,
    0, 0, -2.0f / (GLfloat)far_z, 0,
    -1.0f, 1.0f, -1.0f, 1.0f
  };
  
  GLuint mMvpLocation = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  
  // glDeleteBuffers(1, &ebo);
  // glDeleteBuffers(1, &vbo);
  // 
  // glDeleteVertexArrays(1, &vao);
  
  return 0;
}


/*
 * Draw triangle
 */
void draw_triangle_gl3(GLfloat x1,  GLfloat y1,
                       GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                       GLfloat x2,  GLfloat y2,
                       GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                       GLfloat x3,  GLfloat y3,
                       GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {

  GLfloat vertices[] =
    { x1, y1, c1r, c1g, c1b, c1a,
      x2, y2, c2r, c2g, c2b, c2a,
      x3, y3, c3r, c3g, c3b, c3a };
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


/*
 * Draw image
 */
void draw_image_gl3(Image img) {
}


/*
 * Draw text
 */
void draw_text_gl3(Text txt) {
}
