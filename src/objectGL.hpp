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
  inline GLuint getHandle() const;

protected:
  GLuint handle = 0;
};

inline GLuint objectGL::getHandle() const { return handle; }

#endif
