#ifndef CUBE__HPP_
#define CUBE__HPP_

#include <mesh.hpp>

class cube : public mesh {
public:
  cube(std::shared_ptr<programShader> sProgram, glm::vec3 pos,
       glm::vec3 size = glm::vec3(1.f));

public:
  bool collision(const mesh &m);

public:
  static const std::vector<glm::vec3> verticesCube;
};

#endif
