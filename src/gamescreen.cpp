#include "gamescreen.hpp"

void gamescreen::init() {
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
  for (int i = 0; i < this->activeTiles.size(); i++) {
    tile &xtile = activeTiles[i];
    if (xtile.tIsRendered == true) {
      xtile.Draw(gameRender);
    }
  }
}

void gamescreen::drawFromBoardMap(spriteRender &gameRender) {
  int x = 0;
  for (int i = 0; i < this->board_size_x; i++) {
    for (int j = 0; j < this->board_size_y+5; j++) {
      tile &xtile = activeTiles[x];
      if (tilemap[i][j] != 0) {
        xtile.tIsRendered = tilemap[i][j];
        xtile.Draw(gameRender);
      } else {
        xtile.tIsRendered = 0;
        // Don't Draw
      }
      x++;
    }
  }
}
