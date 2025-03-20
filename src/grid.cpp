#include <grid.hpp>

grid::grid(std::shared_ptr<programShader> sProgram, glm::vec3 pos,
           unsigned int lineCount)
    : mesh(getVerteciesGrid(lineCount), {}, {sProgram}) {}

std::vector<glm::vec3> grid::getVerteciesGrid(unsigned int lineCount) {
  std::vector<glm::vec3> vertecies;

  float step = 1.f / (float)lineCount;

  for (unsigned int i = 1; i < lineCount; ++i) {
    vertecies.push_back({step * (float)i, 0, 0});
    vertecies.push_back({step * (float)i, 0, step * lineCount});

    vertecies.push_back({0, 0, step * (float)i});
    vertecies.push_back({step * lineCount, 0, step * (float)i});
  }

  return vertecies;
}
