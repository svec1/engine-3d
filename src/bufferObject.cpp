#include <bufferObject.hpp>

bufferObject::bufferObject(GLenum _bufferType) : bufferType(_bufferType) {
  glGenBuffers(1, &handle);
}

bufferObject::~bufferObject() { glDeleteBuffers(1, &handle); }

void bufferObject::bind() { glBindBuffer(bufferType, handle); }
void bufferObject::unbind() { glBindBuffer(bufferType, 0); }
