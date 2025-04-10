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
  void setGridVisible(bool _gridVisible = true);

  grid                               &getGrid();
  std::shared_ptr<const physicObject> getObject(unsigned int index);
  bool                                getGridVisible();

public:
  void createObject(float mass, float radius, glm::vec3 pos,
                    glm::vec3 speed = glm::vec3(0.f));
  void deleteObject(unsigned int index);

public:
  void simulation();
  void render(const glm::mat4 &projection, const glm::mat4 &view);

public:
  std::shared_ptr<programShader> sProgram;

  grid   gr{glm::vec3{0, 0, 0}, 320};
  GLuint grLocObjectComponents, grLocGravityConstant;
  bool   gridVisible = true;

  float gravityConstant;

  std::vector<std::shared_ptr<physicObject>> objects;
};

#endif
