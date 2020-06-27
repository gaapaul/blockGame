#ifndef RENDER_TEXT_HPP
#define RENDER_TEXT_HPP
#include <glad/glad.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "stb_image.hpp"
#include "shader.hpp"
#include <map>
//freetype
#include <ft2build.h>
#include FT_FREETYPE_H


struct character {
  GLuint TextureID;
  glm::ivec2 Size;
  glm::ivec2 Bearing;
  long int Advance;
};
class textRender
{
private:
  GLuint VAO, VBO;
  std::map<GLchar, character> characters;
  const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>\n"
    "out vec2 TexCoord;\n"

    "uniform mat4 transform;\n"

    "void main()\n"
    "{\n"
    "    gl_Position = transform*vec4(vertex.xy, 0.0, 1.0);\n"
    "    TexCoord = vertex.zw;\n"
    "} \n\0";
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

  FT_Library ft;
  FT_Face face;
  const char* font = "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf";
  
  Shader textShader;
public:
  void init();
  void renderText(std::string text, glm::vec2 position, glm::vec2 scale, glm::vec3 color);
};


#endif RENDER_TEXT_HPP
