#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm.hpp>

class Shader
{
public:
  // ID
  unsigned int ID;

  //Constructor
  Shader() { }
  //Set the Shader as active with use
  Shader &Use();

  int compile(const char *vertexShaderSource, const char *fragmentShaderSource);
  void transform_4fv(int count, GLboolean transpose, const GLfloat *transform_vec);
  void setColor(const glm::vec3 &colorVec);
};
#endif
