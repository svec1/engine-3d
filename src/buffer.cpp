#include <buffer.hpp>

buffer::buffer(GLuint _count, GLenum _bufferType)
    : count(_count), bufferType(_bufferType) {
  glGenBuffers(count, &handle);
  glBindBuffer(bufferType, handle);
}

buffer::~buffer() { glDeleteBuffers(count, &handle); }
