#include "spriteRender.hpp"
#include "tile.hpp"
#include <glm.hpp>
#ifndef GAME_PIECE_HPP
#define GAME_PIECE_HPP
class gamepiece {
private:
  texture tileTexture;
  glm::vec2 GPSize = glm::vec2(1.0f / 20, 1.0f / 20);
  int tetronimo[7][4][4] = {
      {{0, 0, 1, 1}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 2, 2, 0}, {0, 0, 2, 2}, {0, 0, 0, 0}},
      {{0, 0, 3, 0}, {0, 0, 3, 0}, {0, 3, 3, 0}, {0, 0, 0, 0}},
      {{0, 0, 4, 0}, {0, 0, 4, 0}, {0, 0, 4, 4}, {0, 0, 0, 0}},
      {{0, 0, 5, 0}, {0, 5, 5, 5}, {0, 0, 0, 0}, {0, 0, 0, 0}},
      {{0, 0, 0, 0}, {0, 6, 6, 0}, {0, 6, 6, 0}, {0, 0, 0, 0}},
      {{0, 7, 0, 0}, {0, 7, 0, 0}, {0, 7, 0, 0}, {0, 7, 0, 0}}};

public:
  gamepiece();
  void draw(spriteRender &renderer);
  void create(int Type, int xPos, int yPos);
  void rotatePieceCcw();
  void rotatePieceCw();
  int pieceType;
  float xPosOffset = 2;  // same as board offset
  float yPosOffset = -8; // same as some other offset
  float xPos = 5;
  float yPos = 10;
  glm::vec3 color;
  tile curr_block[4][4];
};
#endif GAME_PIECE_HPP
