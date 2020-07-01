#include "gamepiece.hpp"
gamepiece::gamepiece() {
  this->tileTexture.init("block.jpg");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->curr_block[j][i].tTexture = tileTexture;
    }
  }
}
void gamepiece::draw(spriteRender &renderer) {
  float x;
  float y;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (this->curr_block[j][i].tIsRendered != 0) {
        x = i + this->xPos + this->xPosOffset;
        x = x / 20;
        y = j + this->yPos + this->yPosOffset;
        y = y / 20;
        this->curr_block[j][i].tPosition = glm::vec2(x, y);
        this->curr_block[j][i].Draw(renderer);
      } else {
        this->curr_block[j][i].tPosition = glm::vec2(0.0f, 0.0f);
      }
    }
  }
}

void gamepiece::create(int Type, int xPos, int yPos) {
  this->pieceType = Type;
  // if (this->pieceType == 6) {
  //  this->xPos = 5;
  //} else {
  // this->xPos = 3;
  this->xPos = xPos;
  //}
  // this->yPos = 20-4;
  this->yPos = yPos;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->curr_block[i][j].tSize = this->GPSize;
      if (this->tetronimo[(this->pieceType) - 1][i][j] > 0) {
        this->curr_block[i][j].tIsRendered = Type;
      } else {
        this->curr_block[i][j].tIsRendered = 0;
      }
    }
  }
}
void gamepiece::rotatePieceCw() {
  if (this->pieceType == 6) {
    return;
  }
  int tmp[4][4] = {{0}};
  int x, y;
  // If it is a line we rotate the 4x4 array by 90 degrees
  // If it is not a line and not a square then we rotate the 3x3 array.
  if (this->pieceType == 7) {
    for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
        tmp[x][3 - y] = this->curr_block[y][x].tIsRendered;
      }
    }
  } else {
    for (y = 0; y < 3; y++) {
      for (x = 1; x < 4; x++) {
        tmp[x - 1][3 - y] = this->curr_block[y][x].tIsRendered;
      }
    }
  }

  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      this->curr_block[x][y].tIsRendered = tmp[x][y];
    }
  }
}
void gamepiece::rotatePieceCcw() {
  int tmp[4][4] = {{0}};
  int x, y;
  if (this->pieceType == 6) {
    return;
  }
  if (this->pieceType == 7) {
    for (y = 0; y < 4; y++) {
      for (x = 0; x < 4; x++) {
        tmp[3 - x][y] = this->curr_block[y][x].tIsRendered;
      }
    }
  } else {
    for (y = 0; y < 3; y++) {
      for (x = 1; x < 4; x++) {
        tmp[3 - x][y + 1] = this->curr_block[y][x].tIsRendered;
      }
    }
  }
  for (x = 0; x < 4; x++) {
    for (y = 0; y < 4; y++) {
      this->curr_block[x][y].tIsRendered = tmp[x][y];
    }
  }
}
