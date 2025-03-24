#ifndef GRID__HPP_
#define GRID__HPP_

#include <mesh.hpp>

class grid : public mesh {
public:
  grid(glm::vec3 pos = glm::vec3(1.f), unsigned int _lineCount = 16);

public:
  void init(const dataShaderProgram &&dataSProgram) override final;

public:
  static std::vector<glm::vec3> getVerteciesGrid(unsigned int lineCount);

private:
  unsigned int lineCount;
};

#endif
