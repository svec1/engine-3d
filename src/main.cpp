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

#include <algorithm>
#include <ctime>
#include <iostream>

static camera cam({260, 3, 250});
static bool   gridVisible = true;
static bool   cursorAttention = true;

void error_callback(int error, const char *description) {
  std::cerr << "Error: " << description << std::endl;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  static bool glLineMode = false;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  else if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    if (cursorAttention)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursorAttention = !cursorAttention;
  } else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    if (!glLineMode)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineMode = !glLineMode;
  } else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    gridVisible = !gridVisible;
}

void cursorCallback(GLFWwindow *window, double xPos, double yPos) {
  if (cursorAttention) {
    int wWidth, wHeight;
    glfwGetWindowSize(window, &wWidth, &wHeight);
    cam.cursorCallback(wWidth, wHeight, xPos, yPos);
    glfwSetCursorPos(window, wWidth / 2, wHeight / 2);
  }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  if (int newSpeedMovement = cam.getSpeedMovement() + (8 * yoffset);
      newSpeedMovement >= 0)
    cam.setSpeedMovement(newSpeedMovement);
}

int main(void) {
  GLFWwindow *window;
  GLuint      vertex_buffer;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1280, 960, "engine-3d", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursorCallback);
  glfwSetScrollCallback(window, scrollCallback);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glfwWindowHint(GLFW_SAMPLES, 4);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  io.IniFilename = nullptr;

  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  std::srand(std::time({}));

  double prevTime = glfwGetTime();
  double currentTime, deltaTime;

  resourceManager rManager("res/shaders");

  universe uv(GRAVITY_CONST * 100);

  uv.setProgramsShader(
      rManager.createProgramShader("objectVertexShader.glsl",
                                   "objectFragmentShader.glsl"),
      rManager.createProgramShader("gridVertexShader.glsl",
                                   "gridFragmentShader.glsl"));

  uv.createObject(9000000, 50, {0, 0, 0}, {0, 0.f, 0.05f});

  std::size_t iCountPlanets = 16;
  std::size_t jCountPlanets = 16;

  std::size_t countPlanets = iCountPlanets * jCountPlanets;

  for (std::size_t i = 1; i <= iCountPlanets; ++i) {
    for (std::size_t j = 1; j <= jCountPlanets; ++j) {
      uv.createObject(1000, i, {i * 100 + j, i * 10 + j * 10, j * 100 + i});
    }
  }

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

  while (!glfwWindowShouldClose(window)) {
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::FORWARD);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::BACK);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::LEFT);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::RIGHT);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::TOP);
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      cam.keyboardCallback(cameraAction::BOTTOM);

    if (uv.getGridVisible() != gridVisible)
      uv.setGridVisible(gridVisible);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Camera movement behind the first object
    /*
     glm::vec3 tmpPosObject = uv.getObject(0)->getPos();
     cam.setPos(glm::vec3{tmpPosObject.x, cam.getPos().y, tmpPosObject.z -
     1000});
    */

    glm::mat4 V = cam.getViewMatrix(deltaTime);

    uv.simulation();
    uv.render(P, V);

    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      ImGui::Begin("Simulation information");

      ImGui::Text("%i objects are simulated", countPlanets);
      ImGui::Text("Grid visibility: %i", gridVisible);
      ImGui::Text("Camera position: x=%.1f, y=%.1f, z=%.1f", cam.getPos().x,
                  cam.getPos().y, cam.getPos().z);
      ImGui::Text("Camera movement speed: %.0f", cam.getSpeedMovement());
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);

      ImGui::End();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(1);
}
