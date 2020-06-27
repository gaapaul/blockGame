#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <glad/glad.h>
#include <glm.hpp>
#include "stb_image.hpp"


class texture
{
  public:
    void init(const char *filename);
    void bind();
  private:
    unsigned int ID;
};
#endif

