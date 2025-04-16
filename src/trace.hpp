#ifndef TRACE__HPP
#define TRACE__HPP

#include <mesh.hpp>

class trace : public mesh {
public:
  trace(const glm::vec3 startPos, const std::size_t length = 250);

public:
  void init(const dataShaderProgram &&dataSProgram) override final;

public:
  void addPosition(glm::vec3 pos);

private:
  std::size_t length;
};

#endif
