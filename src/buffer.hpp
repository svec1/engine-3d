#include <objectGL.hpp>

#include <type_traits>

#include <iostream>

class buffer : public objectGL {
public:
  buffer(GLuint _count, GLenum _bufferType);
  ~buffer();

public:
  template <typename T> void loadData(GLuint size, const T *data, GLenum usage);

private:
  GLuint count;
  GLenum bufferType;
};

template <typename T>
void buffer::loadData(GLuint size, const T *data, GLenum usage) {
  glBufferData(bufferType, size, data, usage);
}
