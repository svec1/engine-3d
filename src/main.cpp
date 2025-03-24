#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <resourceManager.hpp>
#include <universe.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <ctime>
#include <iostream>

static const char *fragment_shader_text = "#version 460\n"
                                          "in vec3 VFragColor;\n"
                                          "out vec3 FragColor;\n"
                                          "void main()\n"
                                          "{\n"
                                          "    FragColor = VFragColor;\n"
                                          "}\n";

camera cam({0, 3, 0});

static void error_callback(int error, const char *description) {
  fprintf(stderr, "Error: %s\n", description);
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

  window = glfwCreateWindow(1280, 960, "Simple example", NULL, NULL);
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
  double prevTime = glfwGetTime();

  resourceManager rManager("res/shaders");

  universe uv(0.0006);
  uv.setProgramsShader(
      rManager.createProgramShader("objectVertexShader.glsl",
                                   "objectFragmentShader.glsl"),
      rManager.createProgramShader("gridVertexShader.glsl",
                                   "gridFragmentShader.glsl"));

  uv.createObject(20000, 20, {260, 0, 250});
  uv.createObject(2000, 15, {60, 0, 50}, {0.01, 0, 0.1});

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - prevTime;
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

    glm::mat4 V = cam.getViewMatrix(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    uv.simulation();
    uv.render(P, V);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
