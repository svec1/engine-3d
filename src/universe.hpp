#ifndef UNIVERSE__HPP_
#define UNIVERSE__HPP_

#include <grid.hpp>
#include <physicObject.hpp>
#include <vector>

#define GRAVITY_CONST 0.0000006

class universe {
public:
  universe(const float _gravityConstant = GRAVITY_CONST);

public:
  void setProgramsShader(std::shared_ptr<programShader> _sProgram,
                         std::shared_ptr<programShader> _sProgramGrid);

  glm::vec3 getPosObject(unsigned int index);

public:
  void createObject(float mass, float radius, glm::vec3 pos,
                    glm::vec3 speed = glm::vec3(0.f));
  void deleteObject(unsigned int index);

public:
  void simulation();
  void render(const glm::mat4 &projection, const glm::mat4 &view);

public:
  std::shared_ptr<programShader> sProgram;

  grid gr{glm::vec3{0, 0, 0}, 512};

  float                                      gravityConstant;
  std::vector<std::unique_ptr<physicObject>> objects;
};

#endif
