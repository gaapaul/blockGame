#ifndef TILE_HPP
#define TILE_HPP
#include "spriteRender.hpp"
#include "texture.hpp"
#include <glm.hpp>

// Tile is used to simple render a sprite and store sprite info
// tile has a position,size,texture,rotation, and render flag

class tile {
private:
  glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f),
            orange = glm::vec3(1.0f, 0.6f, 0.0f),
            yellow = glm::vec3(1.0f, 1.0f, 0.0f),
            green = glm::vec3(0.0f, 1.0f, 0.0f),
            tealBlue = glm::vec3(0.0f, .75f, .75f),
            blue = glm::vec3(0.0f, 0.0f, 1.0f),
            purple = glm::vec3(0.7f, 0.0, 1.0f);
  glm::vec3 pieceTypeToColor(int pieceType);

public:
  glm::vec2 tPosition, tSize;
  float tRotation;
  int tIsRendered;
  texture tTexture;
  tile();
  tile(texture spriteTexture, glm::vec2 pos, glm::vec2 size, float rotation,
       int renderType);
  void Draw(spriteRender &renderer);
};

#endif TILE_HPP
