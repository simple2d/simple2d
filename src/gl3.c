// OpenGL 3.3

#include "../include/simple2d.h"

static GLuint shaderProgram;
static GLuint texShaderProgram;


/*
 * Initalize OpenGL
 */
int gl3_init(int width, int height) {
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // TEXTURES: Vertex shader source string
  const GLchar* vertexSource =
    "#version 150 core\n"
    "uniform mat4 u_mvpMatrix;"
    "in vec2 position;"
    "in vec4 color;"
    "in vec2 texcoord;"
    "out vec4 Color;"
    "out vec2 Texcoord;"
    "void main() {"
    "  Color = color;"
    "  Texcoord = texcoord;"
    "  gl_Position = u_mvpMatrix * vec4(position, 0.0, 1.0);"
    "}";
  
  // Fragment shader source string
  const GLchar* fragmentSource =
    "#version 150 core\n"
    "in vec4 Color;"
    "out vec4 outColor;"
    "void main() {"
    "  outColor = Color;"
    "}";
  
  // TEXTURES: Fragment shader source string
  const GLchar* texFragmentSource =
    "#version 150 core\n"
    "in vec4 Color;"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D tex;"
    "void main() {"
    "  outColor = texture(tex, Texcoord) * Color;"
    "}";
  
  // Create Vertex Array Object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  // Create Vertex Buffer Object
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  // Create an element array
  GLuint ebo;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  
  // Load shaders
  GLuint vertexShader = gl_load_shader(GL_VERTEX_SHADER, vertexSource, "GL3 Vertex");
  GLuint fragmentShader = gl_load_shader(GL_FRAGMENT_SHADER, fragmentSource, "GL3 Fragment");
  
  // Link the vertex and fragment shader into a shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  
  // TODO: This error should be handled better
  GLint linked;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
  if (!linked) {
    puts("Error: GL3 shader was not linked!");
  }
  
  // Load texture shaders
  GLuint texFragmentShader = gl_load_shader(GL_FRAGMENT_SHADER, texFragmentSource, "GL3 Texture Fragment");
  
  texShaderProgram = glCreateProgram();
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);
  glBindFragDataLocation(texShaderProgram, 0, "outColor");
  glLinkProgram(texShaderProgram);
  
  // Check if linked
  glGetProgramiv(texShaderProgram, GL_LINK_STATUS, &linked);
  if (!linked) {
    puts("Error: GL3 texture shader was not linked!");
  }
  
  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  
  GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
  glEnableVertexAttribArray(texAttrib);
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  
  // Create and apply an orthographic projection matrix (2D projection)
  // (matrix is given in column-first order)
  GLfloat far_z = 128.0;
  GLfloat orthoMatrix[16] = {
    2.0f / (GLfloat)width, 0, 0, 0,
    0, -2.0f / (GLfloat)height, 0, 0,
    0, 0, -2.0f / far_z, 0,
    -1.0f, 1.0f, -1.0f, 1.0f
  };
  
  glUseProgram(shaderProgram);
  GLuint mMvpLocation = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  glUseProgram(texShaderProgram);
  GLuint texmMvpLocation = glGetUniformLocation(texShaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(texmMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  // TODO: Clean up (do we actually have to do this stuff?)
  // glDeleteShader(vertexShader);
  // glDeleteShader(texFragmentShader);
  // glDeleteShader(fragmentShader);
  // glDeleteProgram(shaderProgram);
  // glDeleteProgram(texShaderProgram);
  // glDeleteVertexArrays(1, &vao);
  // glDeleteBuffers(1, &vbo);
  // glDeleteBuffers(1, &ebo);
  
  return 0;
}


/*
 * Draw triangle
 */
void gl3_draw_triangle(GLfloat x1,  GLfloat y1,
                       GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                       GLfloat x2,  GLfloat y2,
                       GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                       GLfloat x3,  GLfloat y3,
                       GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  // TODO: Probably want to declare this at file scope and make static for performance?
  // Same for other verticies and element arrays below
  GLfloat vertices[] =
    { x1, y1, c1r, c1g, c1b, c1a, 0, 0,
      x2, y2, c2r, c2g, c2b, c2a, 0, 0,
      x3, y3, c3r, c3g, c3b, c3a, 0, 0 };
  
  glUseProgram(shaderProgram);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


static void gl3_draw_texture(int x, int y, int w, int h, 
                             GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                             GLuint texture_id) {
  
  GLfloat vertices[] =
    //    x,     y, r, g, b, a,  tx,  ty
    {     x,     y, r, g, b, a, 0.f, 0.f,    // Top-left
      x + w,     y, r, g, b, a, 1.f, 0.f,    // Top-right
      x + w, y + h, r, g, b, a, 1.f, 1.f,    // Bottom-right
          x, y + h, r, g, b, a, 0.f, 1.f };  // Bottom-left
  
  glUseProgram(texShaderProgram);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}



/*
 * Draw image
 */
void gl3_draw_image(Image img) {
  gl3_draw_texture(img.x, img.y, img.w, img.h, 1.f, 1.f, 1.f, 1.f, img.texture_id);
}


/*
 * Draw text
 */
void gl3_draw_text(Text txt) {
  gl3_draw_texture(txt.x, txt.y, txt.w, txt.h, 
                   txt.color.r, txt.color.g, txt.color.b, txt.color.a, txt.texture_id);
}
