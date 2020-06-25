#include "gamescreen.hpp"
#include <iostream>
void gamescreen::init() {
  std::cout << "abc" << std::endl;
  this->backgroundShader.compile(this->vbShaderSource, this->fbShaderSource);
  std::cout << "abc" << std::endl;
  this->backgroundRender = new spriteRender(this->backgroundShader, "background.jpg");
  this->backgroundTile = new tile(glm::vec2(.1f,-.4f), glm::vec2(.6f, 1.5f), 0,10);
  for (int i = 0; i < this->board_size_x; i++) {
    for (int j = 0; j < this->board_size_y+5; j++) {
      // create vector of tiles in this order.
      if (i == 0 || j == 0 || i == board_size_x - 1 || j > board_size_y-1) {
        tilemap[i][j] = 10;
        this->activeTiles.push_back(
            tile(glm::vec2((float)i / 20 + this->board_offset_x,
                           (float)j / 20 + this->board_offset_y),
                 this->tileSize, 0, 8));

      } else {
        tilemap[i][j] = 0;
        this->activeTiles.push_back(
            tile(glm::vec2((float)i / 20 + this->board_offset_x,
                           (float)j / 20 + this->board_offset_y),
                 this->tileSize, 0, 0));
      }
    }
  }
}

void gamescreen::draw(spriteRender &gameRender) {
  this->backgroundTile->tIsRendered = 10;
  this->backgroundTile->Draw(*this->backgroundRender);
  for (int i = 0; i < this->activeTiles.size(); i++) {
    tile &xtile = activeTiles[i];
    if (xtile.tIsRendered == true) {
      xtile.Draw(*this->backgroundRender);
    }
  }
  std::cout << "here" << std::endl;
  this->backgroundTile->Draw(*this->backgroundRender);
}

void gamescreen::drawFromBoardMap(spriteRender &gameRender) {
  this->backgroundTile->tIsRendered = 11;
  this->backgroundTile->Draw(*this->backgroundRender);
  int x = 0;
  for (int i = 0; i < this->board_size_x; i++) {
    for (int j = 0; j < this->board_size_y+5; j++) {
      tile &xtile = activeTiles[x];
      if (tilemap[i][j] != 0) {
        xtile.tIsRendered = tilemap[i][j];
        xtile.Draw(gameRender);
        //xtile.Draw(*this->backgroundRender);
      } else {
        xtile.tIsRendered = 0;
        // Don't Draw
      }
      x++;
    }
  }
  
}
