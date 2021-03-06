#include "spriteRender.hpp"
#include <iostream>
spriteRender::spriteRender(Shader &shader) {
  this->SpriteShader = shader;
  this->InitSprite();
}

spriteRender::~spriteRender() { glDeleteVertexArrays(1, &this->VAO); }
void spriteRender::InitSprite() {

  GLfloat verts[] = {
      // positions          // texture coords
      -.5,
      .5f,
      0.0f,
      1.0f,
      .5f,
      -.5f,
      1.0f,
      0.0f,
      -.5f,
      -.5f,
      0.0f,
      0.0f,
      // other triangle
      -.5f,
      .5f,
      0.0f,
      1.0f,
      .5f,
      .5f,
      1.0f,
      1.0f,
      .5f,
      -.5f,
      1.0f,
      0.0f,
  };

  int width, height, nrChannels;
  unsigned int VBO;
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // first index location of 0, vertex attributes are composed of 2 values, size
  // of float, space between attributes is 2 * sizeof a float, offset is 0
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(0); // for vertex

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void *)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1); // for texture

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void spriteRender::DrawSprite(texture &renderTexture, glm::vec2 position,
                              float degree, glm::vec2 size, glm::vec3 color) {
  this->SpriteShader.Use();

  glm::mat4 transform = glm::mat4(1.0f);
  // Translate first
  transform = glm::translate(transform, glm::vec3(position, 0.0f));
  // Rotate Next
  transform =
      glm::rotate(transform, glm::radians(degree), glm::vec3(0.0f, 0.0f, 1.0f));
  transform = glm::scale(transform, glm::vec3(size, 1.0f));
  this->SpriteShader.transform_4fv(1, GL_FALSE, glm::value_ptr(transform));
  this->SpriteShader.setColor(color);
  glActiveTexture(GL_TEXTURE0);
  renderTexture.bind();
  glBindVertexArray(this->VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
