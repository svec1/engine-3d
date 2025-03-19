#include <vao.hpp>

vao::vao() { glGenVertexArrays(1, &handle); }
vao::~vao() { glDeleteVertexArrays(1, &handle); }

void vao::bind() { glBindVertexArray(handle); }
void vao::unbind() { glBindVertexArray(0); };

void vao::defineAttrPoint(GLuint index, GLuint size, GLenum type,
                          GLboolean normalized, GLsizei stride,
                          const void *pointer) {
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}
void vao::enable(GLuint index) { glEnableVertexAttribArray(index); }
void vao::disable(GLuint index) { glDisableVertexAttribArray(index); }

