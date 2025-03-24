#include <grid.hpp>

grid::grid(glm::vec3 pos, unsigned int _lineCount) : lineCount(_lineCount) {
  setPos(pos);
}

void grid::init(const dataShaderProgram &&dataSProgram) {
  setData(getVerteciesGrid(lineCount), {}, std::move(dataSProgram));
  setColor(glm::vec3(0.5f, 0.5f, 0.5f));
}

std::vector<glm::vec3> grid::getVerteciesGrid(unsigned int lineCount) {
  std::vector<glm::vec3> vertecies;

  float step = 1.f / (float)lineCount;

  for (unsigned int i = 1; i < lineCount; ++i) {
    for (unsigned int j = 1; j < lineCount; ++j) {
      vertecies.push_back({step * (float)i, 0, step * (j - 1)});
      vertecies.push_back({step * (float)i, 0, step * j});

      vertecies.push_back({step * (j - 1), 0, step * (float)i});
      vertecies.push_back({step * j, 0, step * (float)i});
    }
  }

  return vertecies;
}
