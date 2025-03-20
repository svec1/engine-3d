#ifndef MESH__HPP_
#define MESH__HPP_

#include <glm/glm.hpp>

#include <functional>
#include <string_view>
#include <vector>

#include <bufferObject.hpp>
#include <programShader.hpp>
#include <vao.hpp>

struct metaDataShaderProgram {
  metaDataShaderProgram() = default;
  metaDataShaderProgram(GLuint _lV3Vertex, GLuint _lV3Color,
                        std::string _nameUniformMVP)
      : layoutVec3Vertex(_lV3Vertex), layoutVec3Color(_lV3Color),
        nameUniformMVP(_nameUniformMVP) {}

public:
  GLuint layoutVec3Vertex;
  GLuint layoutVec3Color;
  GLuint locationUniformMVP;

  std::string nameUniformMVP;
};

struct dataShaderProgram {
  dataShaderProgram(std::shared_ptr<programShader> _sProgram,
                    GLuint _lV3Vertex = 0, GLuint _lV3Color = 1,
                    std::string _nameUniformMVP = "MVP")
      : sProgram(_sProgram), mData(_lV3Vertex, _lV3Color, _nameUniformMVP) {}

  dataShaderProgram(std::string_view _vShader, std::string_view _fShader,
                    GLuint _lV3Vertex = 0, GLuint _lV3Color = 1,
                    std::string _nameUniformMVP = "MVP")
      : vertexShader(_vShader), fragmentShader(_fShader),
        mData(_lV3Vertex, _lV3Color, _nameUniformMVP) {}

public:
  std::string vertexShader;
  std::string fragmentShader;

  std::shared_ptr<programShader> sProgram;

  metaDataShaderProgram mData;
};

class mesh {
public:
  mesh() = default;
  mesh(const std::vector<glm::vec3> &_vertecies,
       const std::vector<glm::vec3> &_colors,
       const dataShaderProgram     &&dataSProgram);

public:
  void setPos(glm::vec3 pos);
  void setData(const std::vector<glm::vec3> &_vertecies,
               const std::vector<glm::vec3> &_colors,
               const dataShaderProgram     &&dataSProgram);

  void setVBOData(const std::vector<glm::vec3> *_vertecies,
                  const std::vector<glm::vec3> *_colors);

  void setColors(const std::vector<glm::vec3> &colors);

  glm::vec3                      getPos() const;
  std::vector<glm::vec3>         getTransformVertecies() const;
  std::shared_ptr<programShader> getProgramShader() const;

  static glm::vec3
  getVertex(const std::vector<glm::vec3> &vertecies,
            std::function<bool(const glm::vec3 &v1, const glm::vec3 &v2)> f);

public:
  void initEBO(const std::vector<unsigned int> &indices);

public:
  void move(glm::vec3 pos);
  void rotate(float angle);
  void scale(glm::vec3 scale);

public:
  void draw(const glm::mat4 &projection, const glm::mat4 &view,
            GLenum mode = GL_TRIANGLES);

private:
  void createShaderProgram(const dataShaderProgram &dataSProgram);
  void initVAO();

private:
  glm::mat4 model{1.f};

  std::vector<glm::vec3> vertecies;
  std::vector<glm::vec3> colors;

  std::vector<unsigned int> indices;

  std::shared_ptr<programShader> sProgram;
  metaDataShaderProgram          mData;

  vao          attribObject;
  bufferObject bufferVertecies;
  bufferObject bufferElementsIndex;
  bufferObject bufferColors;

  bool _initEBO = false;
};

#endif
