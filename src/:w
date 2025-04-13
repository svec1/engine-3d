#ifndef RESOURCEMANAGER__HPP_
#define RESOURCEMANAGER__HPP_

#include <programShader.hpp>

class resourceManager {
public:
  resourceManager(std::string_view _directory);

public:
  std::shared_ptr<programShader>
  createProgramShader(std::string_view fileNameVertexShader,
                      std::string_view fileNameFragmentShader);

private:
  std::string directory;
};

#endif
