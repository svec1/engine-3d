#include <camera.hpp>

camera::camera(glm::vec3 _pos) : pos(_pos) {}

void camera::cursorCallback(GLFWwindow *window, double xPos, double yPos) {
  int wWidth, wHeight;

  glfwGetWindowSize(window, &wWidth, &wHeight);
  glfwSetCursorPos(window, wWidth / 2, wHeight / 2);

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
void camera::keyboardCallback(int key) {
  if (key == GLFW_KEY_W)
    tmpPosDirection = direction;
  else if (key == GLFW_KEY_S)
    tmpPosDirection = -direction;
  else if (key == GLFW_KEY_D)
    tmpPosDirection = right;
  else if (key == GLFW_KEY_A)
    tmpPosDirection = -right;
  else if (key == GLFW_KEY_SPACE) {
    pos.y += 2;
    return;
  } else if (key == GLFW_KEY_LEFT_SHIFT) {
    pos.y -= 2;
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
