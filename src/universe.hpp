#ifndef UNIVERSE__HPP_
#define UNIVERSE__HPP_

#include <grid.hpp>
#include <physicObject.hpp>
#include <vector>

class universe {
public:
  universe(const float _gravityConstant = 1);

public:
  void setProgramsShader(std::shared_ptr<programShader> _sProgram,
                         std::shared_ptr<programShader> _sProgramGrid);

public:
  void createObject(float mass, float radius, glm::vec3 pos,
                    glm::vec3 speed = glm::vec3(0.f));
  void deleteObject(unsigned int index);

public:
  void simulation();
  void render(const glm::mat4 &projection, const glm::mat4 &view);

public:
  std::shared_ptr<programShader> sProgram;

  grid gr{glm::vec3{0, 0, 0}, 320};

  float                                      gravityConstant;
  std::vector<std::unique_ptr<physicObject>> objects;
};

#endif
