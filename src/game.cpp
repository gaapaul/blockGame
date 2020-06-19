#include "game.hpp"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
void game::initGame(std::chrono::steady_clock::time_point time) {
  this->gameShader.compile(this->vertexShaderSource,
                           this->fragmentShaderSource);
  this->shadowShader.compile(this->vertexShaderSource,
                            this->fragmentShaderSource2);
  this->ppRender = new spriteRender(this->gameShader);
  this->psRender = new spriteRender(this->shadowShader);
  this->board.init();
  srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
  this->createBag();
  //std::cout << tetronimoBag[0] << std::endl;
  this->tetronimo.create(this->tetronimoBag[0],3,16);
  this->shadowTetronimo.create(this->tetronimoBag[0],3,16);
  this->nextPiece.create(this->tetronimoBag[1], 10,10);
  this->inputKeys[0] = new key(singular, 1.0); //Q
  this->inputKeys[1] = new key(singular, 1.0); //w
  this->inputKeys[2] = new key(singular, 1.0); //E
  this->inputKeys[3] = new key(continuous, 0.12); //A
  this->inputKeys[4] = new key(continuous, 0.12); //S
  this->inputKeys[5] = new key(continuous, 0.12); //D
  this->inputKeys[6] = new key(singular, 1.0); //Space
  this->prevTime = time;
}
void game::createBag()
{
  int randVal;
  int tmp;
  int tmpBag[7] = {};
  for(int i=6; i>0; --i) {
    randVal = rand() % i;
    if(randVal != i) {
      tmp = this->tetronimoBag[randVal];
      tmpBag[i] = this->tetronimoBag[randVal];
      this->tetronimoBag[randVal] = this->tetronimoBag[i];
      this->tetronimoBag[i] = tmp;
    } else {
      tmpBag[i] = this->tetronimoBag[randVal];
    }
  }
  tmpBag[0] = this->tetronimoBag[0];
  std::cout << "Bag" << std::endl;
  for(int i=0; i < 7; i++) {
    this->tetronimoBag[i] = tmpBag[i];
    std::cout << this->tetronimoBag[i] << std::endl;
  }
  std::cout << "Bag" << std::endl;
}

void game::runGame(GLFWwindow *window, std::chrono::steady_clock::time_point time) {
  // game_logic_here
  this->keyboardInput(window, time);
  std::chrono::duration<double> diff = time - this->prevTime;
  if(diff.count() > .75)
  {
    this->prevTime = time;
    this->tetronimo.yPos -= 1;
    //this->checkCollision();
    if (this->checkCollision(&tetronimo)) {
      this->tetronimo.yPos += 1;
      this->maxRotateBeforeFreeze++;

      std::chrono::duration<double> freezeDiff = time - this->freezeTime;

      if(freezeDiff.count() > this->freezeBlockTime || this->maxRotateBeforeFreeze > 4){
        this->freezeBlock();
      }
    } else {
      this->freezeTime = time;
    }
  }
  this->moveShadowBlock();
  this->renderGame();
}

void game::renderGame() {
  this->board.drawFromBoardMap(*this->ppRender);
  this->shadowTetronimo.draw(*this->psRender);
  this->tetronimo.draw(*this->ppRender);
  this->nextPiece.create(this->tetronimoBag[this->positionInBag + 1], 13, 10);
  this->nextPiece.draw(*this->ppRender);
  this->switchPiece.draw(*this->ppRender);
}

void game::moveShadowBlock(){
  this->shadowTetronimo.yPos = this->tetronimo.yPos;
  this->shadowTetronimo.xPos = this->tetronimo.xPos;
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4; x++) {
      if(this->tetronimo.curr_block[x][y].tIsRendered > 0){
        this->shadowTetronimo.curr_block[x][y].tIsRendered = 10;
      } else {
        this->shadowTetronimo.curr_block[x][y].tIsRendered = 0;
      }
    }
  }
  while(this->checkCollision(&shadowTetronimo) == 0){
    this->shadowTetronimo.yPos -= 1;
  }
  this->shadowTetronimo.yPos += 1;
}
bool game::checkCollision(gamepiece *piece) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (piece->curr_block[j][i].tIsRendered &&
          (board.tilemap[i + (int)piece->xPos]
                        [j + (int)piece->yPos] != 0 &&
           board.tilemap[i + (int)piece->xPos]
           [j + (int)piece->yPos] != 10)) {
        return true;
      }
      if ((piece->curr_block[i][j].tIsRendered > 0)) {
        if ((piece->xPos + j) > 10) {
          return true;
        }
        if ((piece->xPos + j) < 1) {
          return true;
        }
        if ((piece->yPos + i) < 1) {
          return true;
        }
      }
    }
  }
  return false;
}

void game::freezeBlock() {
  int currLineWeight = 0;
  int lineWeight[this->board.board_size_y] = {0};
    for (int i = this->tetronimo.xPos; i < (this->tetronimo.xPos + 4); i++) {
      for (int j = this->tetronimo.yPos; j < (this->tetronimo.yPos + 4); j++) {
        if (this->tetronimo
                .curr_block[j - (int)this->tetronimo.yPos]
                           [i - (int)this->tetronimo.xPos]
                .tIsRendered != 0) {
          this->board.tilemap[i][j] = this->tetronimo
            .curr_block[j - (int)this->tetronimo.yPos]
            [i - (int)this->tetronimo.xPos]
            .tIsRendered;
        }
      }
    }
    for (int i = 1; i < this->board.board_size_y - 1; i++) {
      currLineWeight = 0; // set so
      for (int j = 1; j < this->board.board_size_x - 1; j++) {
        if (this->board.tilemap[j][i] == 0) {
          currLineWeight++;
        }
      }
      lineWeight[i] = currLineWeight;
      std::cout << i << "," << currLineWeight << std::endl;
    }
    for (int k = this->board.board_size_y; k > 0; k--) {
      if (lineWeight[k] == 0) {
        for (int i = k; i < this->board.board_size_y - 2; i++) {
          for (int j = 1; j < this->board.board_size_x - 1; j++) {
            this->board.tilemap[j][i] = this->board.tilemap[j][i + 1];
          }
        }
      }
    }
    if(positionInBag != 6) {
      this->positionInBag++;
      this->tetronimo.create(this->tetronimoBag[positionInBag],3,22);
    } else {
      this->positionInBag = 0;
      this->createBag();
      this->tetronimo.create(this->tetronimoBag[positionInBag],3,22);
      positionInBag++;
    }
    if(this->tetronimoSwitchState == SWITCH_LOCKED){
      this->tetronimoSwitchState = SWITCH_LOADED;
      std::cout << "LOADED" << std::endl;
    }
}
int game::keyboardInput(GLFWwindow *window, std::chrono::steady_clock::time_point time) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  // move playerboard
if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && this->inputKeys[KEY_D]->keyPressed(time)) {
    this->tetronimo.xPos += 1;
    if (this->checkCollision(&tetronimo))
      this->tetronimo.xPos -= 1;
 } else if(glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS) {
  this->inputKeys[KEY_D]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && this->inputKeys[KEY_A]->keyPressed(time)) {
    this->tetronimo.xPos -= 1;
    //this->checkCollision();
    if (this->checkCollision(&tetronimo))
      this->tetronimo.xPos += 1;
  } else if(glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS){
    this->inputKeys[KEY_A]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && this->inputKeys[KEY_W]->keyPressed(time)) {
    this->tetronimo.rotatePieceCcw();
    //this->checkCollision();
    if (this->checkCollision(&tetronimo)) {
      this->tetronimo.xPos++;
    }
    if (this->checkCollision(&tetronimo)) {
      this->tetronimo.xPos -= 2;
    }
    if(this->checkCollision(&tetronimo)) {
      this->tetronimo.xPos++;
      this->tetronimo.yPos++;
    }
    if (this->checkCollision(&tetronimo)) {
      this->tetronimo.yPos--;
      this->tetronimo.rotatePieceCw();
    } else {
      this->freezeTime = time;
    }
  } else if(glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS){
    this->inputKeys[KEY_W]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && this->inputKeys[KEY_S]->keyPressed(time)) {
    this->tetronimo.yPos -= 1;
    //this->checkCollision();
    if (this->checkCollision(&tetronimo)) {
      this->tetronimo.yPos += 1;
      this->freezeBlock();
    }
  } else if(glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS){
    this->inputKeys[KEY_S]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && this->inputKeys[KEY_SPACE]->keyPressed(time)) {
    //this->checkCollision();
    while(this->checkCollision(&tetronimo) == 0){
      this->tetronimo.yPos -= 1;
      //this->checkCollision();
    }
    this->tetronimo.yPos += 1;
    this->freezeBlock();
  } else if(glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS){
    this->inputKeys[KEY_SPACE]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && this->inputKeys[KEY_Q]->keyPressed(time)) {

    return 4;
  } else if(glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS){
    this->inputKeys[KEY_Q]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && this->inputKeys[KEY_E]->keyPressed(time)) {
    if(this->tetronimoSwitchState == SWITCH_UNLOADED){
      if(positionInBag != 6) {
        this->switchPiece.create(this->tetronimoBag[positionInBag],-5,10);
        this->switchType = this->tetronimoBag[positionInBag];
        this->positionInBag++;
        this->tetronimo.create(this->tetronimoBag[positionInBag],3,16);
      } else {
        this->switchPiece.create(this->tetronimoBag[positionInBag],-5,10);
        this->switchType = this->tetronimoBag[positionInBag];
        this->tetronimoBag[this->positionInBag] = 0;
        this->createBag();
        this->tetronimo.create(this->tetronimoBag[positionInBag],3,16);
        this->positionInBag++;
      }
    } else if(this->tetronimoSwitchState == SWITCH_LOADED) {
        this->switchPiece.create(this->tetronimoBag[positionInBag],-5,10);
        this->tetronimo.create(this->switchType,3,16);
        this->switchType = this->tetronimoBag[positionInBag];
        std::cout << "From Loaded" << this->switchType << std::endl;
    }
    this->tetronimoSwitchState = SWITCH_LOCKED;
  } else if(glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS){
    this->inputKeys[KEY_E]->keyUnpressed();
 }
  return 6;
}
