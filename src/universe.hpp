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
  void setTraceVisible(bool _traceVisible = true);
  void setGravityConstant(const float _gravityConstant = GRAVITY_CONST);

  std::shared_ptr<const physicObject> getObject(unsigned int index) const;

  bool        getGridVisible() const;
  bool        getTraceVisible() const;
  const float getGravityConstant() const;

public:
  void createObject(float mass, float radius, glm::vec3 pos,
                    glm::vec3 speed = glm::vec3(0.f));
  void deleteObject(unsigned int index);
  void deleteAllObjects();

  void generate(std::size_t countPlanets, std::size_t minMass,
                std::size_t maxMass, std::size_t minDistance);

public:
  void simulation();
  void render(const glm::mat4 &projection, const glm::mat4 &view);

  float calculateWorkSystem();

private:
  std::shared_ptr<programShader> sProgram;

  grid   gr{glm::vec3{0, 0, 0}, 320};
  GLuint grLocObjectComponents, grLocGravityConstant;

  bool gridVisible = true;
  bool traceVisible = true;

  float lastW = 0.f;

  float gravityConstant;

  std::vector<std::shared_ptr<physicObject>> objects;
};

#endif
