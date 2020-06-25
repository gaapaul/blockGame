#ifndef SPRITE_H
#define SPRITE_H
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.hpp"
#include "shader.hpp"

class spriteRender
{
 public:
  spriteRender(Shader &shader);
  ~spriteRender();

  void DrawSprite(glm::vec2 position, float degree, glm::vec2 size, glm::vec3 color);
 private:
  void InitSprite(const char* filename);
  unsigned int VAO;
  unsigned int texture;
  Shader SpriteShader;
};
#endif

