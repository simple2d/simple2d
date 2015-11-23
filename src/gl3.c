// OpenGL 3.3

#include "../include/simple2d.h"

static GLuint shaderProgram;
static GLuint texShaderProgram;
static GLuint elements[] = {
  0, 1, 2,
  2, 3, 0
};

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
int gl3_check_linked(GLuint program) {
  GLint linked;
  
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  
  if (!linked) {
    S2D_Log("GL3 shader was not linked", S2D_ERROR);
    return GL_FALSE;
  }
  
  return GL_TRUE;
}


/*
 * Sets the view and matrix projection
 */
void gl3_set_view(int window_width,       int window_height,
                  int s2d_viewport_width, int s2d_viewport_height) {
  
  glViewport(0, 0, window_width, window_height);
  
  // Set orthographic projection matrix
  orthoMatrix[0] =  2.0f / (GLfloat)s2d_viewport_width;
  orthoMatrix[5] = -2.0f / (GLfloat)s2d_viewport_height;
  
  // Use the program object
  glUseProgram(shaderProgram);
  
  GLuint mMvpLocation = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(mMvpLocation, 1, GL_FALSE, orthoMatrix);
  
  // Use the texture program object
  glUseProgram(texShaderProgram);
  
  GLuint texmMvpLocation = glGetUniformLocation(texShaderProgram, "u_mvpMatrix");
  glUniformMatrix4fv(texmMvpLocation, 1, GL_FALSE, orthoMatrix);
}


/*
 * Initalize OpenGL
 */
int gl3_init(int width, int height) {
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Vertex shader source string
  GLchar vertexSource[] =
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
  GLchar fragmentSource[] =
    "#version 150 core\n"
    "in vec4 Color;"
    "out vec4 outColor;"
    "void main() {"
    "  outColor = Color;"
    "}";
  
  // Fragment shader source string for textures
  GLchar texFragmentSource[] =
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
  
  // Load the vertex and fragment shaders
  GLuint vertexShader = gl_load_shader(GL_VERTEX_SHADER, vertexSource, "GL3 Vertex");
  GLuint fragmentShader = gl_load_shader(GL_FRAGMENT_SHADER, fragmentSource, "GL3 Fragment");
  GLuint texFragmentShader = gl_load_shader(GL_FRAGMENT_SHADER, texFragmentSource, "GL3 Texture Fragment");
  
  // Create the shader program object
  shaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (shaderProgram == 0) {
    gl_print_error("Failed to create shader program");
    return GL_FALSE;
  }
  
  // Create the texture shader program object
  texShaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (texShaderProgram == 0) {
    gl_print_error("Failed to create shader program");
    return GL_FALSE;
  }
  
  // Attach the shader objects to the program object
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);
  
  // Bind the varying out variables to the fragment shader color number
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glBindFragDataLocation(texShaderProgram, 0, "outColor");
  
  // Link the shader program
  glLinkProgram(shaderProgram);
  glLinkProgram(texShaderProgram);
  
  // Check if linked
  gl3_check_linked(shaderProgram);
  gl3_check_linked(texShaderProgram);
  
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
  
  gl3_set_view(width, height, width, height);
  
  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);
  
  return GL_TRUE;
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
  // |x, y coords | colors    | x, y texture coords
    { x    , y    , r, g, b, a, 0.f, 0.f,    // Top-left
      x + w, y    , r, g, b, a, 1.f, 0.f,    // Top-right
      x + w, y + h, r, g, b, a, 1.f, 1.f,    // Bottom-right
      x    , y + h, r, g, b, a, 0.f, 1.f };  // Bottom-left
  
  glUseProgram(texShaderProgram);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


/*
 * Draw image
 */
void gl3_draw_image(Image img) {
  gl3_draw_texture(
    img.x, img.y, img.w, img.h,
    1.f, 1.f, 1.f, 1.f,
    img.texture_id
  );
}


/*
 * Draw text
 */
void gl3_draw_text(Text txt) {
  gl3_draw_texture(
    txt.x, txt.y, txt.w, txt.h, 
    txt.color.r, txt.color.g, txt.color.b, txt.color.a,
    txt.texture_id
  );
}
