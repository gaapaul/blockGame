#include "gamepiece.hpp"
#include "gamescreen.hpp"
#include "shader.hpp"
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
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 outColor;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 0.0f, 1.0f);\n"
    "   outColor = aColor;"
    "}\n\0";
  const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 outColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(outColor, 1.0f);\n"
    "}\n\0";
  const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 outColor;"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(outColor, 0.0f);\n"
    "}\n\0";
  bool gameStatus;
  gamepiece tetronimo;
  gamepiece shadowTetronimo;
  gamepiece nextPiece;
  gamepiece switchPiece;
  gamescreen board;
  key *inputKeys[7];
  Shader gameShader;
  Shader shadowShader;
  double freezeBlockTime = 1;
  int maxRotateBeforeFreeze = 3;
  int tetronimoBag[7] = {1,2,3,4,5,6,7};
  int positionInBag = 0;
  switchStates tetronimoSwitchState = SWITCH_UNLOADED;
  int switchType;
  std::chrono::steady_clock::time_point prevTime;
  std::chrono::steady_clock::time_point freezeTime;
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
