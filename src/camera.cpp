#include <camera.hpp>

camera::camera(glm::vec3 _pos) : pos(_pos) {}

void camera::cursorCallback(int wWidth, int wHeight, double xPos, double yPos) {
  hAngleRad += speedMouseRad * (wWidth / 2 - xPos);
  if (float tmpVAngleRad = speedMouseRad * (wHeight / 2 - yPos);
      (vAngleRad < 1.55 || tmpVAngleRad < 0) &&
      (vAngleRad > -1.57 || tmpVAngleRad > 0))
    vAngleRad += tmpVAngleRad;

  direction =
      glm::normalize(glm::vec3{cos(vAngleRad) * sin(hAngleRad), sin(vAngleRad),
                               cos(vAngleRad) * cos(hAngleRad)});

  right = glm::vec3(sin(hAngleRad - 3.14f / 2.0f), 0,
                    cos(hAngleRad - 3.14f / 2.0f));

  up = -glm::cross(right, direction);
}
void camera::keyboardCallback(cameraAction action) {
  if (action == cameraAction::FORWARD)
    tmpPosDirection = direction;
  else if (action == cameraAction::BACK)
    tmpPosDirection = -direction;
  else if (action == cameraAction::RIGHT)
    tmpPosDirection = right;
  else if (action == cameraAction::LEFT)
    tmpPosDirection = -right;
  else if (action == cameraAction::TOP) {
    pos.y += speedMovement / 64;
    return;
  } else if (action == cameraAction::BOTTOM) {
    pos.y -= speedMovement / 64;
    return;
  }
  pressed = 1;
}

void camera::setSpeedMovement(float _speedMovement) {
  speedMovement = _speedMovement;
}
void camera::setPos(glm::vec3 _pos) { pos = _pos; }

float     camera::getSpeedMovement() { return speedMovement; }
glm::mat4 camera::getViewMatrix(float deltaTime) {
  if (pressed) {
    pos += tmpPosDirection * deltaTime * speedMovement;
    pressed = 0;
  }
  return glm::lookAt(pos, pos + direction, up);
}
glm::vec3 camera::getPos() { return pos; }
