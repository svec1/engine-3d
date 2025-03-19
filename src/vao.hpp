#ifndef VAO__HPP_
#define VAO__HPP_

#include <objectGL.hpp>

class vao : public objectGL {
public:
  vao();
  ~vao();

public:
  void bind();
  void unbind();

  void enable(GLuint index);
  void disable(GLuint index);

  void defineAttrPoint(GLuint index, GLuint size = 3, GLenum type = GL_FLOAT,
                       GLboolean normalized = GL_FALSE, GLsizei stride = 0,
                       const void *pointer = nullptr);
};

#endif
