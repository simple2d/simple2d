// OpenGL 3.3

#if !GLES

#include "../include/simple2d.h"

static GLuint shaderProgram;
static GLuint texShaderProgram;
static GLuint indices[] =
  { 0, 1, 2,
    2, 3, 0 };


/*
 * Applies the projection matrix
 */
void S2D_GL3_ApplyProjection(GLfloat orthoMatrix[16]) {

  // Use the program object
  glUseProgram(shaderProgram);

  glUniformMatrix4fv(
    glGetUniformLocation(shaderProgram, "u_mvpMatrix"),
    1, GL_FALSE, orthoMatrix
  );

  // Use the texture program object
  glUseProgram(texShaderProgram);

  glUniformMatrix4fv(
    glGetUniformLocation(texShaderProgram, "u_mvpMatrix"),
    1, GL_FALSE, orthoMatrix
  );
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
    "in vec4 position;"
    "in vec4 color;"
    "in vec2 texcoord;"
    "out vec4 Color;"
    "out vec2 Texcoord;"
    "void main() {"
    "  Color = color;"
    "  Texcoord = texcoord;"
    "  gl_Position = u_mvpMatrix * position;"
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
void S2D_GL3_DrawTriangle(GLfloat x1, GLfloat y1,
                          GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                          GLfloat x2, GLfloat y2,
                          GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                          GLfloat x3, GLfloat y3,
                          GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  GLfloat vertices[] =
    { x1, y1, r1, g1, b1, a1, 0, 0,
      x2, y2, r2, g2, b2, a2, 0, 0,
      x3, y3, r3, g3, b3, a3, 0, 0 };

  glUseProgram(shaderProgram);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


/*
 * Draw a texture
 */
static void S2D_GL3_DrawTexture(int x, int y, int w, int h,
                                GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                                GLfloat tx1, GLfloat ty1, GLfloat tx2, GLfloat ty2,
                                GLfloat tx3, GLfloat ty3, GLfloat tx4, GLfloat ty4,
                                GLuint texture_id) {

  GLfloat vertices[] =
  //  x, y coords | colors    | x, y texture coords
    { x    , y    , r, g, b, a, tx1, ty1,    // Top-left
      x + w, y    , r, g, b, a, tx2, ty2,    // Top-right
      x + w, y + h, r, g, b, a, tx3, ty3,    // Bottom-right
      x    , y + h, r, g, b, a, tx4, ty4 };  // Bottom-left

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
    img->x, img->y, img->width, img->height,
    img->color.r, img->color.g, img->color.b, img->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    img->texture_id
  );
}


/*
 * Draw sprite
 */
void S2D_GL3_DrawSprite(S2D_Sprite *spr) {
  S2D_GL3_DrawTexture(
    spr->x, spr->y, spr->width, spr->height,
    spr->color.r, spr->color.g, spr->color.b, spr->color.a,
    spr->tx1, spr->ty1, spr->tx2, spr->ty2, spr->tx3, spr->ty3, spr->tx4, spr->ty4,
    spr->img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GL3_DrawText(S2D_Text *txt) {
  S2D_GL3_DrawTexture(
    txt->x, txt->y, txt->width, txt->height,
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    txt->texture_id
  );
}

#endif
