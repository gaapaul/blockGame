#include "shader.hpp"
#include <iostream>

Shader &Shader::Use() {
  glUseProgram(this->ID);
  return *this;
}

int Shader::compile(const char *vertexShaderSource,
                    const char *fragmentShaderSource) {
  unsigned int fragmentShader, vertexShader;
  GLint success = GL_FALSE;
  GLchar infoLog[512];
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    return 0;
  }
  // compile fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  success = GL_FALSE;
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "Error in fragment shader compilation: " << infoLog
              << std::endl;
    return 0;
  }
  this->ID = glCreateProgram();
  glAttachShader(this->ID, vertexShader);
  glAttachShader(this->ID, fragmentShader);
  glLinkProgram(this->ID);
  success = GL_FALSE;
  glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
  if (success == GL_FALSE) {
    glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
    std::cout << "Error In Link: " << infoLog << std::endl;
    return 0;
  }
  // can delete shaders as they are no longer needed
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  return 1;
}
void Shader::transform_4fv(int count, GLboolean transpose,
                           const GLfloat *transform_vec) {
  glUniformMatrix4fv(glGetUniformLocation(this->ID, "transform"), count,
                     transpose, transform_vec);
}


void Shader::setColor(const glm::vec3 &colorVec)
{
  glUniform3fv(glGetUniformLocation(this->ID, "outColor"), 1, &colorVec[0]);
}
