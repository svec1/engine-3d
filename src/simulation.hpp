#ifndef SIMULATION__HPP
#define SIMULATION__HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <resourceManager.hpp>
#include <universe.hpp>

#include <ctime>
#include <iostream>
#include <string_view>

class simulation {
public:
  simulation(GLFWwindow *_window, std::string_view resourcePath = "res/");
  ~simulation() = default;

public:
  void simulationLoop();

private:
  static void errorCallback(int error, const char *description);
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                          int mods);
  static void mouseButtonCallback(GLFWwindow *window, int button, int action,
                                  int mods);
  static void mouseCursorCallback(GLFWwindow *window, double xPos, double yPos);
  static void mouseScrollCallback(GLFWwindow *window, double xoffset,
                                  double yoffset);

private:
  void init();
  void initNewUniverse();

private:
  void subWindowInfo();
  void subWindowTools();

private:
  double prevTime;
  double currentTime, deltaTime;

  float gravityConstant = GRAVITY_CONST;
  int   coeffGravityConstant = 10;

  int countPlanets = 100;
  int minMass = 100, maxMass = 1 / (gravityConstant / 100);
  int minDistance = 100;

  std::vector<float> works;

public:
  static camera *camPtr;
  static bool    simulationInit;
  static bool    gridVisible;
  static bool    traceVisible;
  static bool    cursorAttention;

private:
  GLFWwindow *window;
  ImGuiIO    *io;

  camera    cam;
  glm::mat4 P;

  universe uv;
};

#endif
