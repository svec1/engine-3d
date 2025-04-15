#include <physicObject.hpp>

physicObject::physicObject(glm::vec3 pos, float radius, float _mass)
    : traceObject(pos), mass(_mass), sphere(pos, radius) {}

float     physicObject::getMass() const { return mass; }
glm::vec3 physicObject::getSpeed() const { return speed; }

void physicObject::setSpeed(glm::vec3 _speed) { speed = _speed; }

void physicObject::physicMove(glm::vec3 acceleration) {
  speed += acceleration;
  move(speed);
}
