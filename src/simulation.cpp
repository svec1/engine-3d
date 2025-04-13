#include <simulation.hpp>

camera *simulation::camPtr;
bool    simulation::simulationInit = false;
bool    simulation::gridVisible = true;
bool    simulation::cursorAttention = true;

simulation::simulation(GLFWwindow *_window, std::string_view resourcePath)
    : window(_window), uv(gravityConstant * coeffGravityConstant) {
  init();

  resourceManager rManager(resourcePath);
  rManager.setDirectoryShaders("shaders/");
  uv.setProgramsShader(
      rManager.createProgramShader("objectVertexShader.glsl",
                                   "objectFragmentShader.glsl"),
      rManager.createProgramShader("gridVertexShader.glsl",
                                   "gridFragmentShader.glsl"));
  initNewUniverse();
}

void simulation::keyCallback(GLFWwindow *window, int key, int scancode,
                             int action, int mods) {
  static bool glLineMode = false;

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  else if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    if (!glLineMode)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineMode = !glLineMode;
  } else if (key == GLFW_KEY_G && action == GLFW_PRESS)
    gridVisible = !gridVisible;
}
void simulation::mouseButtonCallback(GLFWwindow *window, int button, int action,
                                     int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    if (cursorAttention)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!cursorAttention)
      glfwSetCursorPos(window, 0, 0);
    cursorAttention = !cursorAttention;
  }
}
void simulation::mouseCursorCallback(GLFWwindow *window, double xPos,
                                     double yPos) {
  if (cursorAttention) {
    int wWidth, wHeight;
    glfwGetWindowSize(window, &wWidth, &wHeight);
    camPtr->cursorCallback(wWidth, wHeight, xPos, yPos);
    glfwSetCursorPos(window, wWidth / 2, wHeight / 2);
  }
}

void simulation::mouseScrollCallback(GLFWwindow *window, double xoffset,
                                     double yoffset) {
  if (int newSpeedMovement = camPtr->getSpeedMovement() + (8 * yoffset);
      newSpeedMovement >= 0)
    camPtr->setSpeedMovement(newSpeedMovement);
}
void simulation::init() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, mouseCursorCallback);
  glfwSetScrollCallback(window, mouseScrollCallback);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460");

  io = &ImGui::GetIO();
  io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io->IniFilename = nullptr;

  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_DEPTH_TEST);

  std::srand(std::time({}));
  prevTime = glfwGetTime();

  uv.setGravityConstant(gravityConstant * coeffGravityConstant);
  camPtr = &cam;
  simulationInit = true;
}

void simulation::initNewUniverse() {
  uv.createObject(90000000, 50, {60, 0, 260}, {0, 0, 0.04f});
  for (std::size_t i = 1; i <= 16; ++i) {
    for (std::size_t j = 1; j <= 16; ++j) {
      uv.createObject(1000, i, {i * 100 + j, i * 10 + j * 10, j * 100 + i});
    }
  }
}

void simulation::subWindowInfo() {
  ImGui::SetNextWindowPos({0, 0});
  ImGui::SetNextWindowSize({360, 180});

  ImGui::Begin("Simulation information");

  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(150, 150, 150, 200));
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / io->Framerate, io->Framerate);
  ImGui::PopStyleColor();

  ImGui::Text("%i objects are simulated", countPlanets);
  ImGui::Text("Gravity constant: %f", uv.getGravityConstant());
  ImGui::Text("Grid visibility: %i", gridVisible);
  ImGui::Text("Camera position: x=%.1f, y=%.1f, z=%.1f", cam.getPos().x,
              cam.getPos().y, cam.getPos().z);
  ImGui::Text("Camera movement speed: %.0f", cam.getSpeedMovement());

  if (works.size())
    ImGui::Text("Work of the simulation system: %f", works[works.size() - 1]);
  ImGui::PlotLines("Graph", works.data(), works.size());

  ImGui::End();
}
void simulation::subWindowTools() {
  ImGui::SetNextWindowPos({0, 180});
  ImGui::SetNextWindowSize({360, 400});

  ImGui::Begin("Tools");

  ImGui::Text("New universe");

  if (ImGui::Button("Generate"))
    uv.generate(countPlanets, minMass, maxMass, minDistance);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::BeginChild("New universe child", {345, 115}, true);

  ImGui::SliderInt("Count planets", &countPlanets, 0, 1024);

  ImGui::SliderInt("Min mass", &minMass, 1, maxMass);
  ImGui::SliderInt("Max mass", &maxMass, 1, 1 / (gravityConstant / 100));

  ImGui::SliderInt("Distance factor", &minDistance, 1, 10000);

  ImGui::EndChild();
  ImGui::PopStyleVar();

  ImGui::Text("Gravitational constant coefficient");
  if (ImGui::SliderInt("##", &coeffGravityConstant, 1, 100))
    uv.setGravityConstant(gravityConstant * coeffGravityConstant);

  ImGui::End();
}

void simulation::simulationLoop() {
  std::size_t countRendering = 0;
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

    // Render simulation
    {
      int wWidth, wHeight;
      glfwGetWindowSize(window, &wWidth, &wHeight);

      P = glm::perspective(80.0f, (float)wWidth / (float)wHeight, 0.1f,
                           10000.0f);
      glm::mat4 V = cam.getViewMatrix(deltaTime);

      uv.simulation();
      uv.render(P, V);
    }

    // Render ImGui subwindows
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      subWindowInfo();
      subWindowTools();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    if (countRendering == 8) {
      if (works.size() == 32)
        works.erase(works.begin());
      works.push_back(uv.calculateWorkSystem());
      countRendering = 0;
    }

    ++countRendering;

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}
