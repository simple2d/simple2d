// OpenGL 3.3

#include "../include/simple2d.h"

static GLuint shaderProgram;
static GLuint texShaderProgram;
static GLuint indices[] = {
  0, 1, 2,
  2, 3, 0
};


/*
 * Applies the matrix projection
 */
void S2D_GL3_ApplyProjection(GLfloat orthoMatrix[16]) {
  
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
int S2D_GL3_Init() {
  
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
  GLuint vertexShader      = S2D_GL_LoadShader(  GL_VERTEX_SHADER,      vertexSource, "GL3 Vertex");
  GLuint fragmentShader    = S2D_GL_LoadShader(GL_FRAGMENT_SHADER,    fragmentSource, "GL3 Fragment");
  GLuint texFragmentShader = S2D_GL_LoadShader(GL_FRAGMENT_SHADER, texFragmentSource, "GL3 Texture Fragment");
  
  // Create the shader program object
  shaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (shaderProgram == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }
  
  // Attach the shader objects to the program object
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  
  // Bind the varying out variables to the fragment shader color number
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  
  // Link the shader program
  glLinkProgram(shaderProgram);
  
  // Check if linked
  S2D_GL_CheckLinked(shaderProgram, "GL3 shader");
  
  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(posAttrib);
  
  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(colAttrib);
  
  // Create the texture shader program object
  texShaderProgram = glCreateProgram();
  
  // Check if program was created successfully
  if (texShaderProgram == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }
  
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);
  
  glBindFragDataLocation(texShaderProgram, 0, "outColor");
  
  glLinkProgram(texShaderProgram);
  
  // Check if linked
  S2D_GL_CheckLinked(texShaderProgram, "GL3 texture shader");
  
  // Specify the layout of the vertex data
  posAttrib = glGetAttribLocation(texShaderProgram, "position");
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(posAttrib);
  
  colAttrib = glGetAttribLocation(texShaderProgram, "color");
  glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(colAttrib);
  
  GLint texAttrib = glGetAttribLocation(texShaderProgram, "texcoord");
  glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(texAttrib);
  
  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);
  
  return GL_TRUE;
}


/*
 * Draw triangle
 */
void S2D_GL3_DrawTriangle(GLfloat x1,  GLfloat y1,
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


/*
 * Draw a texture
 */
static void S2D_GL3_DrawTexture(int x, int y, int w, int h, 
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
  
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


/*
 * Draw image
 */
void S2D_GL3_DrawImage(S2D_Image *img) {
  S2D_GL3_DrawTexture(
    img->x, img->y, img->w, img->h,
    img->color.r, img->color.g, img->color.b, img->color.a,
    img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GL3_DrawText(S2D_Text *txt) {
  S2D_GL3_DrawTexture(
    txt->x, txt->y, txt->w, txt->h, 
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    txt->texture_id
  );
}
