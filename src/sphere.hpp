#ifndef SPHERE__HPP_
#define SPHERE__HPP_

#include <mesh.hpp>

#define CONST_PI 3.1415

#define RINGS 20
#define SECTORS 20

class sphere : public mesh {
public:
  sphere(std::shared_ptr<programShader> sProgram, glm::vec3 pos,
         float _radius = 1);

public:
  bool collision(const sphere &m);

private:
  static std::vector<glm::vec3>    getVerteciesSphere(float radius);
  static std::vector<unsigned int> getIndicesSphere();

private:
  float radius;
};

#endif
