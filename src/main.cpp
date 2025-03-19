#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <camera.hpp>
#include <mesh.hpp>
#include <programShader.hpp>
#include <vao.hpp>
#include <vbo.hpp>

#include <stdio.h>
#include <stdlib.h>

#include <iostream>

static const std::vector<glm::vec3> vertices = {
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, 1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),   glm::vec3(-1.0f, 1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, -1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, 1.0f),   glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),  glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, 1.0f),  glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, -1.0f),   glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),    glm::vec3(1.0f, 1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),  glm::vec3(-1.0f, 1.0f, 1.0f),
};
static const std::vector<glm::vec3> colors = {
    {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
    {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
    {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
    {1.0f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.0f},
    {1.0f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 0.5f, 0.0f},
    {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 1.0f},
    {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
    {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}};
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

  double prevTime = glfwGetTime();

  mesh cube(vertices, colors, {vertex_shader_text, fragment_shader_text});
  mesh cube2(vertices, colors, {cube.getProgramShader()});

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 V;

  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);

  float angle = 0.f;

  cube2.setPos(glm::vec3(3, 2, 2));

  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - prevTime;
    prevTime = currentTime;

    float rotation = deltaTime / 1000.f;
    angle += 360 * (rotation - (long)rotation);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_W);
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_S);
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_A);
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      cam.keyboardCallback(GLFW_KEY_D);

    V = cam.getViewMatrix(deltaTime);

    glClear(GL_COLOR_BUFFER_BIT);

    cube.draw(P, V);
    cube2.draw(P, V);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
