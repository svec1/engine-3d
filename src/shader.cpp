#include <iostream>
#include <shader.hpp>

shader::shader(GLenum _shaderType) : shaderType(_shaderType) {
  handle = glCreateShader(shaderType);
}

shader::shader(GLenum _shaderType, std::string_view source)
    : shaderType(_shaderType) {
  handle = glCreateShader(shaderType);
  setSource(source);
}

shader::~shader() { glDeleteShader(handle); }

void shader::setSource(std::string_view source) {
  const char *buffer_tmp = source.data();
  glShaderSource(handle, 1, &buffer_tmp, NULL);
}
void shader::compile() {
  glCompileShader(handle);

  int  success;
  char infoLog[512];
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(handle, 512, NULL, infoLog);
    std::cout << "ERROR SHADER: COMPILATION FAILED\n" << infoLog << std::endl;
  }
}
GLenum shader::getShaderType() { return shaderType; }
