#include <algorithm>

#include <simulation.hpp>

#define RELEASE_VERSION

#if defined(RELEASE_VERSION) && defined(__unix__)
#define RESOURCES_PATH "/usr/local/bin/"
#else
#define RESOURCES_PATH ""
#endif

static void errorCallback(int error, const char *description) {
  std::cerr << "Error: " << description << std::endl;
}

int main(void) {
  GLFWwindow *window;

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  window = glfwCreateWindow(1920, 1080, "3d-engine", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  gladLoadGL();
  glfwSwapInterval(1);

  glfwWindowHint(GLFW_SAMPLES, 4);

  try {

    simulation sm(window, RESOURCES_PATH "res/");
    sm.simulationLoop();

  } catch (std::exception &excp) {
    std::cout << excp.what() << std::endl;
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  exit(1);
}
