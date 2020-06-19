#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP
#include "tile.hpp"
#include "spriteRender.hpp"
#include <vector>
#include <glm.hpp>

class gamescreen
{
private:
  glm::vec2 tileSize = glm::vec2(0.05f, 0.05f);
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
  std::vector<tile> activeTiles;
  int tilemap[50][50]= {{0}};
  void init();
  void draw(spriteRender &gameRender);
  void drawFromBoardMap(spriteRender &gameRender);
};
#endif GAME_BOARD_HPP
