#include <trace.hpp>

trace::trace(const glm::vec3 startPos, const std::size_t _length)
    : length(_length * 2) {
  getVertecies().push_back(startPos);
  getColors().push_back({glm::vec3{1.f}, 0.f});
}

void trace::init(const dataShaderProgram &&dataSProgram) {
  setData(getVertecies(), getColors(), std::move(dataSProgram));
}

void trace::addPosition(glm::vec3 pos) {
  auto &vertecies = getVertecies();
  auto &colors = getColors();

  if (vertecies.size() == length) {
    vertecies.erase(vertecies.begin(), vertecies.begin() + 2);
    vertecies.push_back(vertecies[vertecies.size() - 1]);
  } else {
    colors.push_back(colors[colors.size() - 1]);
    colors[colors.size() - 1].w += 1.f / (float)length;
  }
  vertecies.push_back(pos);

  updateData();
}
