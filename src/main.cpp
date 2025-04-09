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

static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  cam.setSpeedMovement(cam.getSpeedMovement() + (8 * yoffset));
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
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glfwWindowHint(GLFW_SAMPLES, 4);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  io.IniFilename = nullptr;

  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  std::srand(std::time({}));

  int    countRendered = 0;
  double prevTime = glfwGetTime();
  double currentTime, deltaTime, lastTimeFPS = 0;

  resourceManager rManager("res/shaders");

  universe uv(GRAVITY_CONST * 100);
  uv.setProgramsShader(
      rManager.createProgramShader("objectVertexShader.glsl",
                                   "objectFragmentShader.glsl"),
      rManager.createProgramShader("gridVertexShader.glsl",
                                   "gridFragmentShader.glsl"));

  uv.createObject(9000000, 50, {0, 0, 0}, {0, 0.f, 0.05f});
  // uv.createObject(1000000, 30, {1000, 0, 200}, {0, 0.f, 0.09f});

  for (std::size_t i = 1; i <= 18; ++i) {
    for (std::size_t j = 1; j <= 18; ++j) {
      uv.createObject(1000, i, {i * 100 + j, i * 10 + j * 10, j * 100 + i});
    }
  }

  glm::mat4 P = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 5000.0f);

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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 tmpPosObject = uv.getPosObject(0);
    cam.setPos(
        glm::vec3{tmpPosObject.x, cam.getPos().y, tmpPosObject.z - 1000});

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      static float f = 0.0f;
      static int   counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!"
                                     // and append into it.

      ImGui::Text("This is some useful text."); // Display some text (you can
                                                // use a format strings too)
      ImGui::SliderFloat("float", &f, 0.0f,
                         1.0f); // Edit 1 float using a slider from 0.0f to 1.0f

      if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                   // widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);
      ImGui::End();
    }

    glm::mat4 V = cam.getViewMatrix(deltaTime);

    uv.simulation();
    uv.render(P, V);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();

    if ((int)(currentTime - lastTimeFPS) == 1) {
      lastTimeFPS = currentTime;

      system("clear");

      std::cout << "FPS: " << countRendered << std::endl;
      std::cout << "Camera position: x=" << cam.getPos().x
                << ", y=" << cam.getPos().y << ", z=" << cam.getPos().z
                << std::endl;

      countRendered = 0;
    }

    ++countRendered;
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(1);
}
