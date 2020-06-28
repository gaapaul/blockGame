#include "game.hpp"
#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <time.h>
#include <vector>
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
  this->tetronimo.create(this->tetronimoBag[0],4,22);
  this->pieceOnBoard = this->tetronimoBag[0];
  this->shadowTetronimo.create(this->tetronimoBag[0],4,22);
  this->textRenderer.init();
  this->particleGenerator.init(1);
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
  for(int i=0; i < 7; i++) {
    this->tetronimoBag[i] = tmpBag[i];
  }
}

float game::calcDropTime() {
  switch(this->level){
    case 0:
      return 0.8;
    case 1:
      return 0.716666666666667;
    case 2: 
      return 0.633333333333333;
    case 3: 
      return 0.55;
    case 4: 
      return 0.466666666666667;
    case 5: 
      return 0.383333333333333;
    case 6: 
      return 0.3;
    case 7: 
      return 0.216666666666667;
    case 8: 
      return 0.133333333333333;
    case 9: 
      return 0.1;
    default:
      break;
    }
    if(this->level >= 10 || this->level <= 12)
      return 0.083333333333333;
    else if(this->level>= 13 || this->level <= 15)
      return 0.066666666666667;
    else if(this->level >=16 || this->level <= 18)
      return 0.05;
    else if(this->level >= 19 || this->level <= 28)
      return 0.033333333333333;
    else
      return 0.016666666666667;
}
void game::runGame(GLFWwindow *window, std::chrono::steady_clock::time_point time) {
  // game_logic_here
  this->keyboardInput(window, time);
  std::chrono::duration<double> diff = time - this->prevTime;
  std::chrono::duration<double> fpsdiff = time - this->fpstime;
  this->fpstime = time;
  this->fps = (1.0f/fpsdiff.count() * .7) + (this->fps * .3);
  if(diff.count() > this->calcDropTime())
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
  if(this->visualScore < this->realScore){
    this->visualScore+=rand() % 10;
    if(this->visualScore > this->realScore)
    {
      this->visualScore = this->realScore;
    }
  }
  this->renderGame();
}

void game::renderGame() {
  this->board.drawFromBoardMap(*this->ppRender);
  this->shadowTetronimo.draw(*this->psRender);
  this->tetronimo.draw(*this->ppRender);
  this->nextPiece.create(this->tetronimoBag[this->positionInBag + 1], 13, 10);
  std::cout << "NEXT: " <<  this->tetronimoBag[this->positionInBag + 1] << std::endl;
  this->nextPiece.draw(*this->ppRender);
  this->switchPiece.draw(*this->ppRender);
  this->particleGenerator.updateParticles();
  this->particleGenerator.draw(*this->ppRender);
  this->textRenderer.renderText(std::to_string(this->fps), glm::vec2(0.0f,0.2f), glm::vec2(.5,.5), glm::vec3(1.0,0.0,0.0));
  this->textRenderer.renderText(std::to_string(this->visualScore), glm::vec2(-.25f,0.2f), glm::vec2(.5,.5), glm::vec3(1.0,0.0,0.0));
  this->textRenderer.renderText("LEVEL", glm::vec2(-.35f,0.0f), glm::vec2(.5,.5), glm::vec3(1.0,0.0,0.0));
  this->textRenderer.renderText(std::to_string(this->level), glm::vec2(-0.1f,0.0f), glm::vec2(.5,.5), glm::vec3(1.0,0.0,0.0));
  this->textRenderer.renderText("LINES", glm::vec2(-.35f,-0.2f), glm::vec2(0.5,.5), glm::vec3(1.0,0.0,0.0));
  this->textRenderer.renderText(std::to_string(this->lines), glm::vec2(-0.1f,-0.2f), glm::vec2(.5,.5), glm::vec3(1.0,0.0,0.0));
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
    this->shadowTetronimo.yPos -= .1;
  }
  //this->shadowTetronimo.yPos += 1;
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
  int linesCleared = 0;
  bool loseFlag = false;
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
          if(j - (int)this->tetronimo.xPos > 20){
            loseFlag = true;
          }
        }
      }
    }
    for (int i = 1; i < this->board.board_size_y; i++) {
      currLineWeight = 0; // set so
      for (int j = 1; j < this->board.board_size_x - 1; j++) {
        if (this->board.tilemap[j][i] == 0) {
          currLineWeight++;
        }
      }
      lineWeight[i] = currLineWeight;
    }  
    for (int k = this->board.board_size_y; k > 0; k--) {
      if (lineWeight[k] == 0) {
      for (int j = 1; j < this->board.board_size_x - 1; j++) {
          particleGenerator.createParticles(k,j, this->board.tilemap[j][k]);
        }
      } 
    }      
    for (int k = this->board.board_size_y; k > 0; k--) {
      if (lineWeight[k] == 0) {
        linesCleared++;
        std::cout << "lines Cleared: " << linesCleared << " K " << k << std::endl;
        for (int i = k; i < this->board.board_size_y - 2; i++) {
          for (int j = 1; j < this->board.board_size_x - 1; j++) {
            this->board.tilemap[j][i] = this->board.tilemap[j][i + 1];
          }
        }
      }
    }

    this->addScore(linesCleared,0);
    this->lines+=linesCleared;
    this->updateLevel();
    if(loseFlag){
      this->initGame(this->prevTime);
    }
    if(positionInBag != 5) {
      this->positionInBag++;
      this->tetronimo.create(this->tetronimoBag[positionInBag],3,22);
      this->pieceOnBoard=this->tetronimoBag[positionInBag];
    } else {
      this->positionInBag++;
      this->tetronimo.create(this->tetronimoBag[positionInBag],3,22);
      this->pieceOnBoard=this->tetronimoBag[positionInBag];
      this->positionInBag = 0;
      this->createBag();
    }

    if(this->tetronimoSwitchState == SWITCH_LOCKED){
      this->tetronimoSwitchState = SWITCH_LOADED;
    }
}
void game::updateLevel(){
  //TODO: Update level code
  this->level = this->lines/10;
}
void game::addScore(int linesCleared, int dropLines) {
  switch (linesCleared){
    case 1:
      this->realScore+=40;
      break;
    case 2:
      this->realScore+=100;
      break;
    case 3:
      this->realScore+=300;
      break;
    case 4:
      this->realScore+=1200;
      break;
    default:
      break;
  }
  this->realScore+=dropLines;
}
int game::keyboardInput(GLFWwindow *window, std::chrono::steady_clock::time_point time) {
  int tmp;
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
      //      this->freezeBlock();
    }
  } else if(glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS){
    this->inputKeys[KEY_S]->keyUnpressed();
 }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && this->inputKeys[KEY_SPACE]->keyPressed(time)) {
    //this->checkCollision();
    int score = 0;
    while(this->checkCollision(&tetronimo) == 0){
      this->tetronimo.yPos -= 1;
      score++;
      //this->checkCollision();
    }
    this->tetronimo.yPos += 1;
    this->addScore(0,score);
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
        this->switchPiece.create(this->pieceOnBoard,-5,10);
        this->switchType = this->pieceOnBoard;
        this->positionInBag++;
        this->tetronimo.create(this->tetronimoBag[positionInBag],4,22);
        this->pieceOnBoard=this->tetronimoBag[positionInBag];
      } else {
        this->switchPiece.create(this->pieceOnBoard,-5,10);
        this->switchType = this->pieceOnBoard;
        this->tetronimoBag[this->positionInBag] = 0;
        this->createBag();
        this->tetronimo.create(this->tetronimoBag[positionInBag],3,16);
        this->pieceOnBoard=this->tetronimoBag[positionInBag];
        this->positionInBag++;
      }
    } else if(this->tetronimoSwitchState == SWITCH_LOADED) {
        this->switchPiece.create(this->pieceOnBoard,-5,10);
        this->tetronimo.create(this->switchType,4,22);
        tmp = this->pieceOnBoard;
        this->pieceOnBoard=this->switchType;
        this->switchType = tmp;
    }
    this->tetronimoSwitchState = SWITCH_LOCKED;
  } else if(glfwGetKey(window, GLFW_KEY_E) != GLFW_PRESS){
    this->inputKeys[KEY_E]->keyUnpressed();
 }
  return 6;
}
