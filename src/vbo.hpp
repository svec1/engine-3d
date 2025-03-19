#ifndef VBO__HPP_
#define VBO__HPP_

#include <objectGL.hpp>

#include <type_traits>

class vbo : public objectGL {
public:
  vbo() = delete;

  vbo(GLenum _bufferType);
  ~vbo();

public:
  void bind();
  void unbind();

  template <typename T>
  void loadData(GLuint size, const T *data, GLenum usage = GL_STATIC_DRAW);

private:
  GLenum bufferType;
};

template <typename T>
void vbo::loadData(GLuint size, const T *data, GLenum usage) {
  glBufferData(bufferType, size, data, usage);
}

#endif
