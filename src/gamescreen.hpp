#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP
#include "tile.hpp"
#include "spriteRender.hpp"
#include "shader.hpp"
#include <vector>
#include <glm.hpp>

class gamescreen
{
private:
  glm::vec2 tileSize = glm::vec2(0.05f, 0.05f);
  texture backgroundTexture;
  texture tileTexture;
  texture tileTextureBorder;
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
  Shader backgroundShader;

public:
  float board_offset_x = 0.1;
  float board_offset_y = -0.4;
  int board_size_x=12;
  int board_size_y=22;
  enum GAMESTATE {
                    ACTIVE,
                    GAMEOVER,
                    PAUSE
  };
  tile *backgroundTile;
  std::vector<tile> activeTiles;
  int tilemap[50][50]= {{0}};
  void init();
  void draw(spriteRender &gameRender);
  void drawFromBoardMap(spriteRender &gameRender);
};
#endif GAME_BOARD_HPP
