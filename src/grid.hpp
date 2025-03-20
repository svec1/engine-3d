#ifndef GRID__HPP_
#define GRID__HPP_

#include <mesh.hpp>

class grid : public mesh {
public:
  grid(std::shared_ptr<programShader> sProgram, glm::vec3 pos,
       unsigned int sliceCount = 16);

public:
  static std::vector<glm::vec3> getVerteciesGrid(unsigned int lineCount);
};

#endif
