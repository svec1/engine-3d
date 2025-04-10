#include <resourceManager.hpp>

#include <fstream>
#include <iostream>

resourceManager::resourceManager(std::string_view _directory)
    : directory(_directory) {}

std::shared_ptr<programShader>
resourceManager::createProgramShader(std::string_view fileNameVertexShader,
                                     std::string_view fileNameFragmentShader) {
  std::shared_ptr<programShader> sProgram(new programShader);

  std::ifstream vertexShader((directory + "/") + fileNameVertexShader.data());
  std::ifstream fragmentShader((directory + "/") +
                               fileNameFragmentShader.data());

  char        *tmpBuffer = nullptr;
  unsigned int lengthBuffer = 0;

  if (!vertexShader.is_open()) {
    std::cout << "Couldn't open the file: " << fileNameVertexShader
              << std::endl;
    return {};
  } else if (!fragmentShader.is_open()) {
    std::cout << "Couldn't open the file: " << fileNameFragmentShader
              << std::endl;
    return {};
  }

  std::cout << "Compilation vertex shader: " << fileNameVertexShader
            << std::endl;

  vertexShader.seekg(0, vertexShader.end);
  lengthBuffer = vertexShader.tellg();
  vertexShader.seekg(0, vertexShader.beg);

  tmpBuffer = new char[lengthBuffer + 1];
  vertexShader.read(tmpBuffer, lengthBuffer);
  tmpBuffer[lengthBuffer] = '\0';

  sProgram->createShader(GL_VERTEX_SHADER, tmpBuffer);
  delete[] tmpBuffer;

  std::cout << "Compilation fragment shader: " << fileNameFragmentShader
            << std::endl;

  fragmentShader.seekg(0, fragmentShader.end);
  lengthBuffer = fragmentShader.tellg();
  fragmentShader.seekg(0, fragmentShader.beg);

  tmpBuffer = new char[lengthBuffer + 1];
  tmpBuffer[lengthBuffer] = '\0';
  fragmentShader.read(tmpBuffer, lengthBuffer);
  sProgram->createShader(GL_FRAGMENT_SHADER, tmpBuffer);
  delete[] tmpBuffer;

  sProgram->link();

  return sProgram;
}
