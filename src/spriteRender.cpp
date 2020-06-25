#include "spriteRender.hpp"
#include <iostream>
spriteRender::spriteRender(Shader &shader) {
  this->SpriteShader = shader;
  this->InitSprite();
}

spriteRender::~spriteRender() { glDeleteVertexArrays(1, &this->VAO); }
void spriteRender::InitSprite(const char* filename) {

  GLfloat verts[] = {// positions          // texture coords
                     0.0, 1.0,0.0f,1.0f,
                     1.0f, 0.0f,1.0f,0.0f,
                     0.0f, 0.0f,0.0f,0.0f,
                     //other triangle
                     0.0f,1.0f,0.0f,1.0f,
                     1.0f,1.0f,1.0f,1.0f,
                     1.0f, 0.0f,1.0f,0.0f,};
  //Position vec2, //Colors vec3, //TexCoords vec2
  /* GLfloat verts[] = {
  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
    0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
  -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
  };*/
  int width, height, nrChannels = 3;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  unsigned int VBO;
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // first index location of 0, vertex attributes are composed of 2 values, size
  // of float, space between attributes is 2 * sizeof a float, offset is 0
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(0);// for vertex

  // glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, 7 * sizeof(GLfloat), (void*)(4*sizeof(GLfloat)));
  // glEnableVertexAttribArray(1);// for color

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2*sizeof(GLfloat)));
  glEnableVertexAttribArray(1); //for texture

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenTextures(1, &this->texture);
  glBindTexture(GL_TEXTURE_2D, this->texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if(data){ //Check if data is acceptable
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  this->SpriteShader.Use();
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glUniform1i(glGetUniformLocation(this->SpriteShader.ID, "ourTexture"),0);
}

void spriteRender::DrawSprite(glm::vec2 position, float degree, glm::vec2 size,
                              glm::vec3 color) {

  //std::cout << "here" << std::endl;
  glm::mat4 transform = glm::mat4(1.0f);
  // Translate first
  transform = glm::translate(transform, glm::vec3(position, 0.0f));
  // Rotate Next
  transform =
      glm::rotate(transform, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
  transform = glm::scale(transform, glm::vec3(size, 1.0f));
  this->SpriteShader.Use();
  this->SpriteShader.transform_4fv(1, GL_FALSE, glm::value_ptr(transform));
  this->SpriteShader.setColor(color);
  // TODO color later
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->texture);

  
  glBindVertexArray(this->VAO);
  //std::cout << "a" << std::endl;
  glDrawArrays(GL_TRIANGLES, 0,6);
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
