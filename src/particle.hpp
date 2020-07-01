#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "shader.hpp"
#include "stb_image.hpp"
#include "tile.hpp"
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
// Contains class particle and particleGen:
// Particle contains info on a generated tile which last a lifespan and has a
// velocity. Particle Generator creates and update particles

class particle {
public:
  glm::vec2 velocity = glm::vec2(0.0, 0.0);
  int lifespan = 0;
  tile particleSprite;
  float rotation;
};

class particleGen {
private:
  glm::vec2 tileSize = glm::vec2(0.05f, 0.05f);
  texture tileTexture;
  const char *vbShaderSource =
      "#version 330 core\n"
      "layout (location = 0) in vec2 aPos;\n"
      "layout (location = 1) in vec2 aTexCoord;\n"
      "out vec2 TexCoord;\n"
      "uniform mat4 transform;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = transform * vec4(aPos, 0.0f, 1.0f);\n"
      "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
      "}\n\0";
  const char *fbShaderSource =
      "#version 330 core\n"
      "in vec2 TexCoord;\n"
      "out vec4 FragColor;\n"
      "uniform vec3 outColor;\n"
      "uniform sampler2D ourTexture;\n"
      "void main()\n"
      "{\n"
      "   FragColor =  texture(ourTexture, TexCoord) * vec4(outColor, 1.0f);\n"
      "}\n\0";
  Shader particleShader;
  int particlesPerBlock;

public:
  float board_offset_x = 0.1;
  float board_offset_y = -0.4;
  int board_size_x = 12;
  int board_size_y = 22;
  int rowIndex = 0;
  std::vector<particle> particles;
  void init(int particlesDensity);
  void draw(spriteRender &gameRender);
  void createParticles(float yPosition, float xPosition, int color);
  void updateParticles();
};
#endif PARTICLE_HPP
