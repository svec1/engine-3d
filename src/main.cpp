#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <cube.hpp>
#include <grid.hpp>
#include <sphere.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>
#include <iostream>

static const char *vertex_shader_text =
    "#version 460\n"
    "layout (location = 0) in vec3 vPos;\n"
    "layout (location = 1) in vec3 vClr;\n"
    "out vec3 VFragColor;\n"
    "uniform mat4 MVP;\n"
    "void main()\n"
    "{\n"
    "    VFragColor = vClr;\n"
    "    gl_Position = MVP * vec4(vPos, 1.0);\n"
    "}";

static const char *fragment_shader_text = "#version 460\n"
                                          "in vec3 VFragColor;\n"
                                          "out vec3 FragColor;\n"
                                          "void main()\n"
                                          "{\n"
                                          "    FragColor = VFragColor;\n"
                                          "}\n";

camera cam({0, 0, 5});

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

  double prevTime = glfwGetTime();

  std::shared_ptr<programShader> sProgram(new programShader);
  sProgram->createShader(GL_VERTEX_SHADER, vertex_shader_text);
  sProgram->createShader(GL_FRAGMENT_SHADER, fragment_shader_text);
  sProgram->link();

  grid grid1(sProgram, {}, 12);

  sphere sphere1(sProgram, glm::vec3{0, 1.f, 2.f});
  sphere sphere2(sProgram, glm::vec3{0.f, 0.f, 0.f});

  sphere1.setColors({400, glm::vec3(0.6f, 0.2f, 0.5f)});
  sphere2.setColors({400, glm::vec3(0.3f, 0.6f, 0.2f)});

  grid1.scale(glm::vec3{10.f, 1.f, 10.f});

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);

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

    glm::mat4 V = cam.getViewMatrix(deltaTime);

    if (sphere1.collision(sphere2))
      std::cout << "collision of sphere 1 and sphere 2\r";

    sphere1.move(glm::vec3(0, -0.001f, -0.01f));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sphere1.draw(P, V);
    // sphere2.draw(P, V);

    grid1.draw(P, V, GL_LINES);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
