#ifndef SPRITE_H
#define SPRITE_H
#include "shader.hpp"
#include "stb_image.hpp"
#include "texture.hpp"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Class which can draw a sprite
class spriteRender {
public:
  spriteRender(Shader &shader);
  ~spriteRender();

  void DrawSprite(texture &renderTexture, glm::vec2 position, float degree,
                  glm::vec2 size, glm::vec3 color);

private:
  void InitSprite();
  unsigned int VAO;
  // unsigned int texture;
  Shader SpriteShader;
};
#endif
