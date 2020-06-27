#include "renderText.hpp"
#include <iostream>
#include <vector>
void textRender::init(){
  GLuint VBO;
  int width, height, nrChannels;

  std::cout << "render string" << std::endl;
    this->textShader.compile(this->vertexShaderSource, this->fragmentShaderSource);
    this->textShader.Use();
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1,&this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("block.jpg", &width, &height, &nrChannels, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);// for vertex

    // glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 7 * sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));
    // glEnableVertexAttribArray(1);// for color

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    if(FT_Init_FreeType(&this->ft)){
      std::cout << "Cannot Init Freetype" << std::endl;
      while(1){};
    }
    if(FT_New_Face(this->ft, "/usr/share/fonts/truetype/abyssinica/AbyssinicaSIL-R.ttf", 0, &this->face)) {
      std::cout << "Failed to load the font" << std::endl;
      while(1){};
    }
    
    FT_Set_Pixel_Sizes(face,0,48);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    std::cout << "render string" << std::endl;
    for (GLubyte c = 0; c < 128; c++){
      if(FT_Load_Char(this->face, c, FT_LOAD_RENDER)){
        std::cout << "Failed to load Glyph" << std::endl;
        while(1) {};
      }
      GLuint texture;
      std::cout << "render string" << std::endl;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      
      // Set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      /*glTexImage2D(GL_TEXTURE_2D,
                   0,
                   GL_RED,
                   face->glyph->bitmap.width,
                   face->glyph->bitmap.rows,
                   0,
                   GL_RED,
                   GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer
                   );
      //glGenerateMipmap(GL_TEXTURE_2D);
      */
       
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      character newCharacter = {
                                texture,
                                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                                face->glyph->advance.x
      };
      this->textShader.Use();
      glBindTexture(GL_TEXTURE_2D, texture);
      glUniform1i(glGetUniformLocation(this->textShader.ID, "ourTexture"),0);
      this->characters.insert(std::pair<GLchar, character>(c, newCharacter));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(this->face);
    FT_Done_FreeType(this->ft);

      std::cout << "render string" << std::endl;

}

void textRender::renderText(std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color)
{
  std::cout << "render string" << std::endl;
  this->textShader.Use();
  this->textShader.setColor(color);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(this->VAO);
  std::string::const_iterator c;
  float xCurr = position.x;
  for (c = text.begin(); c != text.end(); c++){
     int i = *c;
     std::cout << i << " ; " << *c << std::endl;
     character ch = this->characters[*c];
     std::cout <<"textureID: " << ch.TextureID << std::endl;
     std::cout << "xcurr " << xCurr << std::endl;
     float xpos = xCurr;
     float ypos = position.y;

     float h = ch.Size.x * scale.x;
     float w = ch.Size.y * scale.y;


     GLfloat verts[] = {// positions          // texture coords
                        0.0, 1.0f,0.0f,1.0f,
                        1.0f, 0.0f,1.0f,0.0f,
                        0.0f, 0.0f,0.0f,0.0f,
                        //other triangle
                        0.0f,1.0f,0.0f,1.0f,
                        1.0f,1.0f,1.0f,1.0f,
                        1.0f, 0.0f,1.0f,0.0f,};
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    xCurr += ch.Advance * scale.x;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D,0);
}

