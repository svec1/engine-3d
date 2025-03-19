#ifndef PROGRAMSHADER__HPP_
#define PROGRAMSHADER__HPP_

#include <memory>
#include <vector>

#include <shader.hpp>

class programShader : public objectGL {
public:
  programShader();
  ~programShader();

public:
  void createShader(GLenum shaderType, std::string_view source);
  void deleteShader(GLenum shaderType);

  void link();
  void use();

  void dump();

public:
  GLuint getLocUniform(std::string_view nameUniform);

private:
  std::vector<std::unique_ptr<shader>> shaders;
};

#endif
