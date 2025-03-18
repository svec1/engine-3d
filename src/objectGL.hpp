#ifndef OBJECTGL__HPP_
#define OBJECTGL__HPP_

#include <glad/glad.h>

class objectGL {
  objectGL(const objectGL &);
  const objectGL &operator=(const objectGL &);

protected:
  objectGL() = default;
  ~objectGL() = default;

public:
  inline GLuint getHandle();

protected:
  GLuint handle = 0;
};

inline GLuint objectGL::getHandle() { return handle; }

#endif
