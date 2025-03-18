#include <programShader.hpp>

programShader::programShader() { handle = glCreateProgram(); }
programShader::~programShader() { glDeleteProgram(handle); }

void programShader::createShader(GLenum shaderType, std::string_view source) {
  shaders.push_back(std::make_unique<shader>(shaderType, source));
  shaders[shaders.size() - 1]->compile();
  glAttachShader(handle, shaders[shaders.size() - 1]->getHandle());
}
void programShader::deleteShader(GLenum shaderType) {
  for (auto it = shaders.begin(); it < shaders.end(); ++it)
    if (it->get()->getShaderType() == shaderType) {
      shaders.erase(it);
      break;
    }
}
void programShader::link() { glLinkProgram(handle); }
void programShader::use() { glUseProgram(handle); }
