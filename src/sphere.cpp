#include <sphere.hpp>

sphere::sphere(glm::vec3 pos, float _radius) : radius(_radius) { setPos(pos); }

float sphere::getRadius() { return radius; }

void sphere::init(const dataShaderProgram &&dataSProgram) {
  setData(getVerteciesSphere(radius), {}, std::move(dataSProgram));
  initEBO(getIndicesSphere());
}
bool sphere::collision(const sphere &m) {
  return radius + m.radius > glm::length(getPos()) - glm::length(m.getPos());
}
std::vector<glm::vec3> sphere::getVerteciesSphere(float radius) {
  std::vector<glm::vec3> vertecies;

  float deltaTheta = 1.f / (float)(RINGS - 1);
  float deltaPhi = 1.f / (float)(SECTORS - 1);

  for (int i = 0; i < RINGS; ++i) {
    for (int j = 0; j < SECTORS; ++j) {
      float th = i * deltaTheta * CONST_PI;
      float ph = j * deltaPhi * 2 * CONST_PI;

      float x = radius * cos(ph) * sin(th);
      float y = radius * sin(ph) * sin(th);
      float z = radius * cos(th);

      vertecies.push_back({x, y, z});
    }
  }

  return vertecies;
}
std::vector<unsigned int> sphere::getIndicesSphere() {
  std::vector<unsigned int> indices;

  for (int i = 0; i < RINGS - 1; ++i) {
    for (int j = 0; j < SECTORS - 1; ++j) {
      unsigned int currentIndex = i * SECTORS + j;
      unsigned int nextIndex = currentIndex + SECTORS;

      indices.push_back(currentIndex);
      indices.push_back(nextIndex);
      indices.push_back(currentIndex + 1);

      indices.push_back(nextIndex);
      indices.push_back(nextIndex + 1);
      indices.push_back(currentIndex + 1);
    }
  }
  return indices;
}
