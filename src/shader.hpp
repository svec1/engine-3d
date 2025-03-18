#ifndef SHADER__HPP_
#define SHADER__HPP_

#include <objectGL.hpp>

#include <string>
#include <string_view>

class shader : public objectGL {
public:
  shader() = delete;
  ~shader();

  shader(GLenum _shaderType);
  shader(GLenum _shaderType, std::string_view source);

public:
  void setSource(std::string_view source);
  void compile();

  GLenum getShaderType();

private:
  GLenum shaderType;
};

#endif
