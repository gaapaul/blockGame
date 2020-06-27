#include "texture.hpp"
#include <iostream>
void texture::init(const char *filename)
{ 
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

  glGenTextures(1, &this->ID);
  glBindTexture(GL_TEXTURE_2D, this->ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  if(data){ //Check if data is acceptable
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "Failed to load texture" << std::endl;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::bind(){
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, this->ID);
}