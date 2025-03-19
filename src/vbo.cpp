#include <vbo.hpp>

vbo::vbo(GLenum _bufferType) : bufferType(_bufferType) {
  glGenBuffers(1, &handle);
}

vbo::~vbo() { glDeleteBuffers(1, &handle); }

void vbo::bind() { glBindBuffer(bufferType, handle); }
void vbo::unbind() { glBindBuffer(bufferType, 0); }
