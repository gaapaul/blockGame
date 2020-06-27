#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glad/glad.h>
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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
  void    SetInteger  (const char *name, int value, bool useShader = false);
  void    SetVector3f (const char *name, float x, float y, float z, bool useShader = false);
  void    SetVector3f (const char *name, const glm::vec3 &value, bool useShader = false);
  void    SetVector4f (const char *name, float x, float y, float z, float w, bool useShader = false);
  void    SetVector4f (const char *name, const glm::vec4 &value, bool useShader = false);
  void    SetMatrix4  (const char *name, const glm::mat4 &matrix, bool useShader = false);
};
#endif
