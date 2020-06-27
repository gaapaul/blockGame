#include "renderText.hpp"
#include <iostream>
#include <vector>
void textRender::init(){
  GLuint VBO;
  int width, height, nrChannels;

  unsigned char *data = stbi_load("block.jpg", &width, &height, &nrChannels, 0);

  this->textShader.compile(this->vertexShaderSource, this->fragmentShaderSource);
  this->textShader.Use();
  this->textShader.SetMatrix4("projection", glm::ortho(1.0f, static_cast<float>(1), static_cast<float>(1), 1.0f), true);
  this->textShader.SetInteger("text", 0);
  // configure VAO/VBO for texture quads
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  glBindVertexArray(this->VAO);
  glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  this->characters.clear();
  if(FT_Init_FreeType(&this->ft)){
    std::cout << "Cannot Init Freetype" << std::endl;
    while(1){};
  }
  if(FT_New_Face(this->ft, "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf", 0, &this->face)) {
    std::cout << "Failed to load the font" << std::endl;
    while(1){};
  }
  
  FT_Set_Pixel_Sizes(face,0,48);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  for (GLubyte c = 0; c < 128; c++){
    if(FT_Load_Char(this->face, c, FT_LOAD_RENDER)){
      std::cout << "Failed to load Glyph" << std::endl;
      while(1) {};
    }
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
    character newCharacter = {
                              texture,
                              glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                              glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                              face->glyph->advance.x
    };
    this->characters.insert(std::pair<GLchar, character>(c, newCharacter));
    this->textShader.Use();
    //glBindTexture(GL_TEXTURE_2D, texture);
    //glUniform1i(glGetUniformLocation(this->textShader.ID, "ourTexture"),0);
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(this->face);
  FT_Done_FreeType(this->ft);
}

void textRender::renderText(std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color)
{
  this->textShader.Use();
  this->textShader.setColor(glm::vec3(color));
  glm::mat4 transform = glm::mat4(1.0f);
  // Translate first
  transform = glm::translate(transform, glm::vec3(position,0.0f));
  // Rotate Next
  transform =
      glm::rotate(transform, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::scale(transform, glm::vec3(scale, 1.0f));
  this->textShader.Use();
  this->textShader.transform_4fv(1, GL_FALSE, glm::value_ptr(transform));
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->VAO);
  std::string::const_iterator c;
  float xCurr = position.x;
  for (c = text.begin(); c != text.end(); c++){
     int i = *c;
     character ch = this->characters[*c];
     float xpos = xCurr;
     float ypos = .5;

     float h = .1 * 1;
     float w = .1 * 1;


    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 0.0f },
        { xpos,     ypos,       0.0f, 0.0f },

        { xpos,     ypos + h,   0.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 0.0f }
    };
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    xCurr += .1;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D,0);
}

