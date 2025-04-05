#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <resourceManager.hpp>
#include <universe.hpp>

#include <algorithm>
#include <ctime>
#include <iostream>

camera cam({260, 3, 250});

static void error_callback(int error, const char *description) {
  std::cerr << "Error: " << description << std::endl;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
  static bool glLineMode = false;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    if (!glLineMode)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineMode = !glLineMode;
  }
}

static void cursorCallback(GLFWwindow *window, double xPos, double yPos) {
  cam.cursorCallback(window, xPos, yPos);
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

  glfwSetKeyCallback(window, key_callback);
  glfwSetCursorPosCallback(window, cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glfwWindowHint(GLFW_SAMPLES, 4);

  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  std::srand(std::time({}));

  int    countRendered = 0;
  double prevTime = glfwGetTime();
  double currentTime, deltaTime, lastTimeFPS = 0;

  float speedSimulation = 1.f;

  resourceManager rManager("res/shaders");

  universe uv(GRAVITY_CONST * 100);
  uv.setProgramsShader(
      rManager.createProgramShader("objectVertexShader.glsl",
                                   "objectFragmentShader.glsl"),
      rManager.createProgramShader("gridVertexShader.glsl",
                                   "gridFragmentShader.glsl"));

  // uv.createObject(9000000, 20, {260, 0, 250});
  uv.createObject(9000000, 10, {260, 0, 250});

  for (std::size_t i = 1; i <= 10; ++i) {
    for (std::size_t j = 1; j <= 10; ++j) {
      uv.createObject(10, i, {i * 100 + j, 0, j * 100 + i}, {0.1f, 0, 0.2f});
    }
  }

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

  while (!glfwWindowShouldClose(window)) {
    currentTime = glfwGetTime();
    deltaTime = currentTime - prevTime;
    prevTime = currentTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_W);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_S);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_A);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_D);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_SPACE);
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_LEFT_SHIFT);
    // else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    // speedSimulation *= 1.05f;
    // else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    // speedSimulation /= 1.05f;

    glm::mat4 V = cam.getViewMatrix(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uv.simulation();
    uv.render(P, V);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if ((int)(currentTime - lastTimeFPS) == 1) {
      lastTimeFPS = currentTime;

      system("clear");

      std::cout << "FPS: " << countRendered << std::endl;
      std::cout << "Camera position: x=" << cam.getPos().x
                << ", y=" << cam.getPos().y << ", z=" << cam.getPos().z
                << std::endl;
      std::cout << "Speed simulation: " << speedSimulation << std::endl;

      countRendered = 0;
    }

    ++countRendered;
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(1);
}
