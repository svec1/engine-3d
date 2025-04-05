#ifndef CAMERA__HPP_
#define CAMERA__HPP_

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class camera {
public:
  camera(glm::vec3 _pos);

public:
  void cursorCallback(GLFWwindow *window, double xPos, double yPos);
  void keyboardCallback(int key);

public:
  glm::mat4 getViewMatrix(float deltaTime);
  glm::vec3 getPos();

private:
  glm::vec3 pos;

  glm::vec3 direction, right, up;
  glm::vec3 tmpPosDirection;

  float hAngleRad = 0;
  float vAngleRad = 0;

  float speedMovement = 128;
  float speedMouseRad = 0.0006f;

  bool pressed = 0;
};

#endif
