// OpenGL ES 2.0

#include "../include/simple2d.h"

#if GLES

static GLuint vbo;  // our primary vertex buffer object (VBO)
static GLuint vboSize;  // size of the VBO in bytes
static GLfloat *vboData;  // pointer to the VBO data
static GLfloat *vboDataCurrent;  // pointer to the data for the current vertices
static GLuint vboDataIndex = 0;  // index of the current object being rendered
static GLuint vboObjSize = 1000;  // number of objects the VBO can store

// Triangle shader
static GLuint shaderProgram;  // triangle shader program
static GLuint positionLocation;
static GLuint colorLocation;

// Texture shader
static GLuint texShaderProgram;  // texture shader program
static GLuint texPositionLocation;
static GLuint texColorLocation;
static GLuint texCoordLocation;
static GLuint texSamplerLocation;

static GLushort indices[] =  // indices for rendering textured quads
  { 0, 1, 2,
    2, 3, 0 };


/*
 * Applies the projection matrix
 */
void S2D_GLES_ApplyProjection(GLfloat orthoMatrix[16]) {

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
 * Initalize OpenGL ES
 */
int S2D_GLES_Init() {

  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Vertex shader source string
  GLchar vertexSource[] =
    // Uniforms used by the vertex shader
    "uniform mat4 u_mvpMatrix;"  // projection matrix

    // Input attributes to the vertex shader
    "attribute vec4 a_position;"  // position value
    "attribute vec4 a_color;"     // vertex color
    "attribute vec2 a_texcoord;"  // texture coordinates

    // Varying variables, output to the fragment shader
    "varying vec4 v_color;"     // vertex color
    "varying vec2 v_texcoord;"  // texture coordinates

    "void main() {"
    // Send the color and texture coordinates right through to the fragment shader
    "  v_color = a_color;"
    "  v_texcoord = a_texcoord;"
    // Transform the vertex position using the projection matrix
    "  gl_Position = u_mvpMatrix * a_position;"
    "}";

  // Fragment shader source string
  GLchar fragmentSource[] =
    // GPU float calculation precision, medium
    "precision mediump float;"

    // Input color from vertex shader
    "varying vec4 v_color;"

    "void main() {"
    "  gl_FragColor = v_color;"  // pass the color right through
    "}";

  // Fragment shader source string for textures
  GLchar texFragmentSource[] =
    // GPU float calculation precision, medium
    "precision mediump float;"

    // Input vertex color from vertex shader
    "varying vec4 v_color;"         // input color from vertex shader
    "varying vec2 v_texcoord;"      // input texture coordinates
    "uniform sampler2D s_texture;"  // 2D texture unit

    "void main() {"
    // Apply the texture unit, texture coordinates, and color
    "  gl_FragColor = texture2D(s_texture, v_texcoord) * v_color;"
    "}";

  // Create a vertex array object
  GLuint vao;
  glGenVertexArraysOES(1, &vao);
  glBindVertexArrayOES(vao);

  // Create a vertex buffer object and allocate data
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  vboSize = vboObjSize * sizeof(GLfloat) * 24;
  vboData = (GLfloat *) malloc(vboSize);
  vboDataCurrent = vboData;

  // Load the vertex and fragment shaders
  GLuint vertexShader      = S2D_GL_LoadShader(  GL_VERTEX_SHADER,      vertexSource, "GLES Vertex");
  GLuint fragmentShader    = S2D_GL_LoadShader(GL_FRAGMENT_SHADER,    fragmentSource, "GLES Fragment");
  GLuint texFragmentShader = S2D_GL_LoadShader(GL_FRAGMENT_SHADER, texFragmentSource, "GLES Texture Fragment");

  // Triangle Shader //

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

  // Link the shader program
  glLinkProgram(shaderProgram);

  // Check if linked
  S2D_GL_CheckLinked(shaderProgram, "GLES shader");

  // Specify the layout of the position vertex data...
  GLint positionLocation = glGetAttribLocation(shaderProgram, "a_position");
  glEnableVertexAttribArray(positionLocation);
  glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

  // ...and the color vertex data
  GLint colorLocation = glGetAttribLocation(shaderProgram, "a_color");
  glEnableVertexAttribArray(colorLocation);
  glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

  // Texture Shader //

  // Create the texture shader program object
  texShaderProgram = glCreateProgram();

  // Check if program was created successfully
  if (texShaderProgram == 0) {
    S2D_GL_PrintError("Failed to create shader program");
    return GL_FALSE;
  }

  // Attach the shader objects to the program object
  glAttachShader(texShaderProgram, vertexShader);
  glAttachShader(texShaderProgram, texFragmentShader);

  // Link the shader program
  glLinkProgram(texShaderProgram);

  // Check if linked
  S2D_GL_CheckLinked(texShaderProgram, "GLES texture shader");

  // Specify the layout of the position vertex data...
  texPositionLocation = glGetAttribLocation(texShaderProgram, "a_position");
  glVertexAttribPointer(texPositionLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
  glEnableVertexAttribArray(texPositionLocation);

  // ...and the color vertex data...
  texColorLocation = glGetAttribLocation(texShaderProgram, "a_color");
  glVertexAttribPointer(texColorLocation, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(texColorLocation);

  // ...and the texture coordinates
  GLint texCoordLocation = glGetAttribLocation(texShaderProgram, "a_texcoord");
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(texCoordLocation);

  // Get the 2D texture sampler location
  texSamplerLocation = glGetUniformLocation(texShaderProgram, "s_texture");

  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);

  // If successful, return true
  return GL_TRUE;
}


/*
 * Render the vertex buffer and reset it
 */
void S2D_GLES_FlushBuffers() {

  // Use the triangle shader program
  glUseProgram(shaderProgram);

  // Bind to the vertex buffer object and update its data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vboDataIndex * 24, vboData);

  // Render all the triangles in the buffer
  glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(vboDataIndex * 3));

  // Reset the buffer object index and data pointer
  vboDataIndex = 0;
  vboDataCurrent = vboData;
}


/*
 * Draw triangle
 */
void S2D_GLES_DrawTriangle(GLfloat x1, GLfloat y1,
                           GLfloat r1, GLfloat g1, GLfloat b1, GLfloat a1,
                           GLfloat x2, GLfloat y2,
                           GLfloat r2, GLfloat g2, GLfloat b2, GLfloat a2,
                           GLfloat x3, GLfloat y3,
                           GLfloat r3, GLfloat g3, GLfloat b3, GLfloat a3) {

  GLfloat vertices[] =
    { x1, y1, 0.f,
      x2, y2, 0.f,
      x3, y3, 0.f };

  GLfloat colors[] =
    { r1, g1, b1, a1,
      r2, g2, b2, a2,
      r3, g3, b3, a3 };

  glUseProgram(shaderProgram);

  // Load the vertex position
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, vertices);
  glEnableVertexAttribArray(positionLocation);

  // Load the colors
  glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(colorLocation);

  // draw
  glDrawArrays(GL_TRIANGLES, 0, 3);



  // LEFT OFF HERE //

  // If buffer is full, flush it
  if (vboDataIndex >= vboObjSize) S2D_GLES_FlushBuffers();

  // Set the triangle data into a formatted array
  GLfloat vertices[] =
    { x1, y1, r1, g1, b1, a1, 0, 0,
      x2, y2, r2, g2, b2, a2, 0, 0,
      x3, y3, r3, g3, b3, a3, 0, 0 };

  // Copy the vertex data into the current position of the buffer
  memcpy(vboDataCurrent, vertices, sizeof(vertices));

  // Increment the buffer object index and the vertex data pointer for next use
  vboDataIndex++;
  vboDataCurrent = (GLfloat *)((char *)vboDataCurrent + (sizeof(GLfloat) * 24));

}


/*
 * Draw a texture
 */
static void S2D_GLES_DrawTexture(int x, int y, int w, int h,
                                 GLfloat angle, GLfloat rx, GLfloat ry,
                                 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                                 GLfloat tx1, GLfloat ty1, GLfloat tx2, GLfloat ty2,
                                 GLfloat tx3, GLfloat ty3, GLfloat tx4, GLfloat ty4,
                                 GLuint texture_id) {

  S2D_GL_Point v1 = { .x = x,     .y = y     };
  S2D_GL_Point v2 = { .x = x + w, .y = y     };
  S2D_GL_Point v3 = { .x = x + w, .y = y + h };
  S2D_GL_Point v4 = { .x = x,     .y = y + h };

  // Rotate vertices
  if (angle != 0) {
    v1 = S2D_RotatePoint(v1, angle, rx, ry);
    v2 = S2D_RotatePoint(v2, angle, rx, ry);
    v3 = S2D_RotatePoint(v3, angle, rx, ry);
    v4 = S2D_RotatePoint(v4, angle, rx, ry);
  }

  GLfloat vertices[] =
  //  x, y coords | x, y texture coords
    { v1.x, v1.y,   0.f, tx1, ty1,
      v2.x, v2.y,   0.f, tx2, ty2,
      v3.x, v3.y,   0.f, tx3, ty3,
      v4.x, v4.y,   0.f, tx4, ty4 };

  GLfloat colors[] =
    { r, g, b, a,
      r, g, b, a,
      r, g, b, a,
      r, g, b, a };

  glUseProgram(texShaderProgram);

  // Load the vertex position
  glVertexAttribPointer(texPositionLocation, 3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), vertices);
  glEnableVertexAttribArray(texPositionLocation);

  // Load the colors
  glVertexAttribPointer(texColorLocation, 4, GL_FLOAT, GL_FALSE, 0, colors);
  glEnableVertexAttribArray(texColorLocation);

  // Load the texture coordinate
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), &vertices[3]);
  glEnableVertexAttribArray(texCoordLocation);

  // Bind the texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  // Set the sampler texture unit to 0
  glUniform1i(texSamplerLocation, 0);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}


/*
 * Draw image
 */
void S2D_GLES_DrawImage(S2D_Image *img) {
  S2D_GLES_DrawTexture(
    img->x, img->y, img->width, img->height,
    img->rotate, img->rx, img->ry,
    img->color.r, img->color.g, img->color.b, img->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    img->texture_id
  );
}


/*
 * Draw sprite
 */
void S2D_GLES_DrawSprite(S2D_Sprite *spr) {
  S2D_GLES_DrawTexture(
    spr->x, spr->y, spr->width, spr->height,
    spr->rotate, spr->rx, spr->ry,
    spr->color.r, spr->color.g, spr->color.b, spr->color.a,
    spr->tx1, spr->ty1, spr->tx2, spr->ty2, spr->tx3, spr->ty3, spr->tx4, spr->ty4,
    spr->img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GLES_DrawText(S2D_Text *txt) {
  S2D_GLES_DrawTexture(
    txt->x, txt->y, txt->width, txt->height,
    txt->rotate, txt->rx, txt->ry,
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    txt->texture_id
  );
}

#endif
