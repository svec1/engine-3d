#ifndef PHYSICOBJECT__HPP_
#define PHYSICOBJECT__HPP_

#include <sphere.hpp>
#include <trace.hpp>

class physicObject : public sphere {
public:
  physicObject(glm::vec3 pos, float radius = 1.f, float _mass = 1.f);
  ~physicObject() = default;

public:
  void physicMove(glm::vec3 acceleration);

public:
  float     getMass() const;
  glm::vec3 getSpeed() const;

  void setSpeed(glm::vec3 _speed);

public:
  trace traceObject;

private:
  float     mass = 0.f;
  glm::vec3 speed{0.f};
};

#endif
