// OpenGL ES 2.0

#include "../include/simple2d.h"

// Triangle shader
static GLuint shaderProgram;
static GLuint positionLocation;
static GLuint colorLocation;

// Texture shader
static GLuint texShaderProgram;
static GLuint texPositionLocation;
static GLuint texColorLocation;
static GLuint texCoordLocation;
static GLuint samplerLocation;

static GLuint indices[] =
  { 0, 1, 2,
    2, 3, 0 };


/*
 * Applies the matrix projection
 */
void S2D_GLES_ApplyProjection(GLfloat orthoMatrix[16]) {
  
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
 * Initalize OpenGL ES
 */
int S2D_GLES_Init() {
  
  // Enable transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // Vertex shader source string
  GLchar vertexSource[] =
    // uniforms used by the vertex shader
    "uniform mat4 u_mvpMatrix;"   // model view and projection matrix
    
    // attributes input to the vertex shader
    "attribute vec4 a_position;"  // position value
    "attribute vec4 a_color;"     // input vertex color
    "attribute vec2 a_texcoord;"  // input texture
    
    // varying variables, input to the fragment shader
    "varying vec4 v_color;"       // output vertex color
    "varying vec2 v_texcoord;"    // output texture
    
    "void main()"
    "{"
    "  v_color = a_color;"
    "  v_texcoord = a_texcoord;"
    "  gl_Position = u_mvpMatrix * a_position;"
    "}";
  
  // Fragment shader source string
  GLchar fragmentSource[] =
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    
    "void main()"
    "{"
    "  gl_FragColor = v_color;"
    "}";
  
  // Fragment shader source string for textures
  GLchar texFragmentSource[] =
    // input vertex color from vertex shader
    "varying vec4 v_color;"
    "varying vec2 v_texcoord;"
    "uniform sampler2D s_texture;"
    
    "void main()"
    "{"
    "  gl_FragColor = texture2D(s_texture, v_texcoord) * v_color;"
    "}";
  
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
  
  // Get the attribute locations
  positionLocation = glGetAttribLocation(shaderProgram, "a_position");
  colorLocation    = glGetAttribLocation(shaderProgram, "a_color");
  
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
  
  // Get the attribute locations
  texPositionLocation = glGetAttribLocation(texShaderProgram, "a_position");
  texColorLocation    = glGetAttribLocation(texShaderProgram, "a_color");
  texCoordLocation    = glGetAttribLocation(texShaderProgram, "a_texcoord");
  
  // Get the sampler location
  samplerLocation = glGetUniformLocation(texShaderProgram, "s_texture");
  
  // Clean up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDeleteShader(texFragmentShader);
  
  return GL_TRUE;
}


/*
 * Draw triangle
 */
void S2D_GLES_DrawTriangle(GLfloat x1,  GLfloat y1,
                           GLfloat c1r, GLfloat c1g, GLfloat c1b, GLfloat c1a,
                           GLfloat x2,  GLfloat y2,
                           GLfloat c2r, GLfloat c2g, GLfloat c2b, GLfloat c2a,
                           GLfloat x3,  GLfloat y3,
                           GLfloat c3r, GLfloat c3g, GLfloat c3b, GLfloat c3a) {
  
  GLfloat vVertices[] =
    { x1, y1, 0.f,
      x2, y2, 0.f,
      x3, y3, 0.f };
  
  GLfloat vColors[] =
    { c1r, c1g, c1b, c1a,
      c2r, c2g, c2b, c2a,
      c3r, c3g, c3b, c3a };
  
  glUseProgram(shaderProgram);
  
  // Load the vertex position
  glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
  glEnableVertexAttribArray(positionLocation);
  
  // Load the colors
  glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, vColors);
  glEnableVertexAttribArray(colorLocation);
  
  // draw
  glDrawArrays(GL_TRIANGLES, 0, 3);
}


/*
 * Draw a texture
 */
static void S2D_GLES_DrawTexture(int x, int y, int w, int h,
                                 GLfloat r, GLfloat g, GLfloat b, GLfloat a,
                                 GLfloat tx1, GLfloat ty1, GLfloat tx2, GLfloat ty2,
                                 GLfloat tx3, GLfloat ty3, GLfloat tx4, GLfloat ty4,
                                 GLuint texture_id) {
  
  GLfloat vVertices[] =
    { x,     y,     0.f, tx1, ty1,
      x + w, y,     0.f, tx2, ty2,
      x + w, y + h, 0.f, tx3, ty3,
      x,     y + h, 0.f, tx4, ty4 };
  
  GLfloat vColors[] =
    { r, g, b, a,
      r, g, b, a,
      r, g, b, a,
      r, g, b, a };
  
  glUseProgram(texShaderProgram);
  
  // Load the vertex position
  glVertexAttribPointer(texPositionLocation, 3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), vVertices);
  glEnableVertexAttribArray(texPositionLocation);
  
  // Load the colors
  glVertexAttribPointer(texColorLocation, 4, GL_FLOAT, GL_FALSE, 0, vColors);
  glEnableVertexAttribArray(texColorLocation);
  
  // Load the texture coordinate
  glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), &vVertices[3]);
  glEnableVertexAttribArray(texCoordLocation);
  
  // Bind the texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  
  // Set the sampler texture unit to 0
  glUniform1i(samplerLocation, 0);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
}


/*
 * Draw image
 */
void S2D_GLES_DrawImage(S2D_Image *img) {
  S2D_GLES_DrawTexture(
    img->x, img->y, img->w, img->h,
    img->color.r, img->color.g, img->color.b, img->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    img->texture_id
  );
}


/*
 * Draw sprite
 */
void S2D_GLES_DrawSprite(S2D_Sprite *spr) {
  S2D_GL3_DrawTexture(
    spr->x, spr->y, spr->w, spr->h,
    spr->img->color.r, spr->img->color.g, spr->img->color.b, spr->img->color.a,
    spr->tx1, spr->ty1, spr->tx2, spr->ty2, spr->tx3, spr->ty3, spr->tx4, spr->ty4,
    spr->img->texture_id
  );
}


/*
 * Draw text
 */
void S2D_GLES_DrawText(S2D_Text *txt) {
  S2D_GLES_DrawTexture(
    txt->x, txt->y, txt->w, txt->h, 
    txt->color.r, txt->color.g, txt->color.b, txt->color.a,
    0.f, 0.f, 1.f, 0.f, 1.f, 1.f, 0.f, 1.f,
    txt->texture_id
  );
}
