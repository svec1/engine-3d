#include <cube.hpp>

const std::vector<glm::vec3> cube::verticesCube = {
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, -1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, 1.0f),  glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, -1.0f),   glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(-1.0f, 1.0f, 1.0f),
};

cube::cube(std::shared_ptr<programShader> sProgram, glm::vec3 pos,
           glm::vec3 size)
    : mesh(verticesCube, {}, {sProgram}) {
  setPos(pos);
  scale(size);
}

bool cube::collision(const mesh &m) {
  auto m1 = getTransformVertecies();
  auto m2 = m.getTransformVertecies();

  auto min = [](const glm::vec3 &v1, const glm::vec3 &v2) {
    return v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z;
  };
  auto max = [](const glm::vec3 &v1, const glm::vec3 &v2) {
    return v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z;
  };

  glm::vec3 m1Min = getVertex(m1, min);
  glm::vec3 m1Max = getVertex(m1, max);

  glm::vec3 m2Min = getVertex(m2, min);
  glm::vec3 m2Max = getVertex(m2, max);

  return (m1Min.x <= m2Max.x && m1Min.y <= m2Max.y && m1Min.z <= m2Max.z) &&
         (m1Max.x >= m2Min.x && m1Max.y >= m2Min.y && m1Max.z >= m2Min.z);
}
