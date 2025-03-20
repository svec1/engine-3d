#ifndef BUFFEROBJECT__HPP_
#define BUFFEROBJECT__HPP_

#include <objectGL.hpp>

#include <type_traits>

class bufferObject : public objectGL {
public:
  bufferObject() = delete;

  bufferObject(GLenum _bufferType);
  ~bufferObject();

public:
  void bind();
  void unbind();

  template <typename T>
  void loadData(GLuint size, const T *data, GLenum usage = GL_STATIC_DRAW);

private:
  GLenum bufferType;
};

template <typename T>
void bufferObject::loadData(GLuint size, const T *data, GLenum usage) {
  glBufferData(bufferType, size, data, usage);
}

#endif
