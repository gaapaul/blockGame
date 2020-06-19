#define GLEW_STATIC
#include "game.hpp"
#include "gamepiece.hpp"
#include "gamescreen.hpp"
#include "shader.hpp"
#include "spriteRender.hpp"
#include <glad/glad.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <chrono>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
int processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos, 1.0f);\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\n\0";
const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
    "}\n\0";
const char *fragmentShaderSource2 =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 1.0f, 0.0f, .5f);\n"
    "}\n\0";
int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  game tgame;
  tgame.initGame(std::chrono::steady_clock::now());
  unsigned int microseconds = 100000;
  while (!glfwWindowShouldClose(window)) {
    //usleep(microseconds);
    // tgame.checkCollision();
    //tgame.keyboardInput(window, std::chrono::steady_clock::now());
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //tgame.renderGame();
    tgame.runGame(window, std::chrono::steady_clock::now());
    // tetgame.drawFromBoardMap(sRender);

    // block.draw(pRender);
    // tile2.tPosition = glm::vec2(x,y);
    /*for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        tile &xtile = activeTiles[i];
        glm::vec2 pos = xtile.tPosition;
        x_i = pos.x;
        y_i = pos.y;
        x_i += .01 * (float)y;
        y_i += .01 * (float)x;

        std::cout << x_i << std::endl;

        xtile.tPosition = glm::vec2(x_i, y_i);
        // xtile.tRotation = r + x * 1000;
        xtile.Draw(sRender);
      }
    }*/
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

int processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  // move playerboard
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    return 0;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    return 1;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    return 2;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    return 3;
  }
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
    return 4;
  }
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    return 5;
  }
  return 6;
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
