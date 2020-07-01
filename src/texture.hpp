#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include "stb_image.hpp"
#include <glad/glad.h>
#include <glm.hpp>

// Texture must be init before use
// Texture is then bound before sprite is drawn
class texture {
public:
  void init(const char *filename);
  void bind();

private:
  unsigned int ID;
};
#endif
