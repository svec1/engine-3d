#ifndef CAMERA__HPP_
#define CAMERA__HPP_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class cameraAction { FORWARD = 0, BACK, LEFT, RIGHT, TOP, BOTTOM };

class camera {
public:
  camera(glm::vec3 _pos);

public:
  void cursorCallback(int wWidth, int wHeight, double xPos, double yPos);
  void keyboardCallback(cameraAction action);

public:
  void setSpeedMovement(float _speedMovement);
  void setPos(glm::vec3 _pos);

  float     getSpeedMovement();
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
