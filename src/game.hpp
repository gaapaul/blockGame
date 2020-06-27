#include "gamepiece.hpp"
#include "gamescreen.hpp"
#include "shader.hpp"
#include "renderText.hpp"
#include "spriteRender.hpp"
#include "key.hpp"
#include <chrono>
#include <glfw3.h>

enum keyList {
               KEY_Q,
               KEY_W,
               KEY_E,
               KEY_A,
               KEY_S,
               KEY_D,
               KEY_SPACE
};
enum switchStates {
                  SWITCH_UNLOADED,
                  SWITCH_LOADED,
                  SWITCH_LOCKED
};
class game
{
private:
  //int keys[1024];
  spriteRender *ppRender;
  spriteRender *psRender;

  const char *vertexShaderSource =
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
  const char *fragmentShaderSource =
    "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "out vec4 FragColor;\n"
    "uniform vec3 outColor;\n"
    "uniform sampler2D ourTexture;\n"
    "void main()\n"
    "{\n"
    "   FragColor =  texture(ourTexture, TexCoord) * vec4(outColor, 1.0f);\n"
    "}\n\0";
  const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 outColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(outColor, 0.0f);\n"
    "}\n\0";
  int realScore = 0;
  int visualScore = 0;
  int level=10;
  int lines=100;
  int pieceOnBoard = 0;
  bool gameStatus;
  gamepiece tetronimo;
  gamepiece shadowTetronimo;
  gamepiece nextPiece;
  gamepiece switchPiece;
  gamescreen board;
  key *inputKeys[7];
  Shader gameShader;
  Shader shadowShader;
  textRender textRenderer;
  double freezeBlockTime = 1;
  int maxRotateBeforeFreeze = 3;
  int tetronimoBag[7] = {1,2,3,4,5,6,7};
  int positionInBag = 1;
  switchStates tetronimoSwitchState = SWITCH_UNLOADED;
  int switchType;
  int fps;
  std::chrono::steady_clock::time_point prevTime;
  std::chrono::steady_clock::time_point fpstime;
  std::chrono::steady_clock::time_point freezeTime;
  void addScore(int linesCleared, int dropLines);
  void updateLevel();
  float calcDropTime();
public:
  void initGame(std::chrono::steady_clock::time_point);
  void freezeBlock();
  void createBag();
  void runGame(GLFWwindow *window, std::chrono::steady_clock::time_point);
  void renderGame();
  void moveShadowBlock();
  int keyboardInput(GLFWwindow *window, std::chrono::steady_clock::time_point time);
  bool checkCollision(gamepiece *piece);
};
