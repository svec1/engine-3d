#include <physicObject.hpp>

physicObject::physicObject(glm::vec3 pos, float radius, float _mass)
    : mass(_mass), sphere(pos, radius) {}

float     physicObject::getMass() { return mass; }
glm::vec3 physicObject::getSpeed() { return speed; }

void physicObject::setSpeed(glm::vec3 _speed) { speed = _speed; }

void physicObject::physicMove(glm::vec3 acceleration) {
  speed += acceleration;
  move(speed);
}
