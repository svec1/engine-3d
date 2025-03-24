#include <mesh.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

mesh::mesh()
    : bufferVertecies(GL_ARRAY_BUFFER),
      bufferElementsIndex(GL_ELEMENT_ARRAY_BUFFER),
      bufferColors(GL_ARRAY_BUFFER) {}

void mesh::createShaderProgram(const dataShaderProgram &dataSProgram) {
  if (dataSProgram.vertexShader.empty())
    sProgram = dataSProgram.sProgram;
  else {
    sProgram = std::shared_ptr<programShader>(new programShader);

    sProgram->createShader(GL_VERTEX_SHADER, dataSProgram.vertexShader);
    sProgram->createShader(GL_FRAGMENT_SHADER, dataSProgram.fragmentShader);
    sProgram->link();
  }

  mData = dataSProgram.mData;
  mData.locationUniformVP = sProgram->getLocUniform(mData.nameUniformVP);
  mData.locationUniformM = sProgram->getLocUniform(mData.nameUniformM);
}
void mesh::initVAO() {
  attribObject.bind();

  bufferVertecies.bind();
  bufferVertecies.loadData(vertecies.size() * sizeof(glm::vec3),
                           vertecies.data());
  attribObject.defineAttrPoint(mData.layoutVec3Vertex, 3, GL_FLOAT, GL_FALSE,
                               sizeof(glm::vec3));
  bufferVertecies.unbind();

  bufferColors.bind();
  bufferColors.loadData(colors.size() * sizeof(glm::vec3), colors.data());
  attribObject.defineAttrPoint(mData.layoutVec3Color, 3, GL_FLOAT, GL_FALSE,
                               sizeof(glm::vec3));
  bufferColors.unbind();

  attribObject.unbind();
}
void mesh::initEBO(const std::vector<unsigned int> &_indices) {
  indices = _indices;

  attribObject.bind();

  bufferElementsIndex.bind();
  bufferElementsIndex.loadData(indices.size() * sizeof(unsigned int),
                               indices.data());

  bufferElementsIndex.unbind();
  attribObject.unbind();

  _initEBO = true;
}
void mesh::setPos(glm::vec3 pos) {
  model = glm::translate(glm::mat4(1.f), pos);
}
glm::vec3              mesh::getPos() const { return model[3]; }
std::vector<glm::vec3> mesh::getTransformVertecies() const {
  std::vector<glm::vec3> transformVertecies;
  for (const auto &vertex : vertecies)
    transformVertecies.push_back(model * glm::vec4(vertex, 1.f));
  return transformVertecies;
}
std::shared_ptr<programShader> mesh::getProgramShader() const {
  return sProgram;
}

void mesh::move(glm::vec3 nPos) { model = glm::translate(model, nPos); }
void mesh::rotate(float angle) {
  model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1.f, 0));
}
void mesh::scale(glm::vec3 scale) { model = glm::scale(model, scale); }

void mesh::setData(const std::vector<glm::vec3> &_vertecies,
                   const std::vector<glm::vec3> &_colors,
                   const dataShaderProgram     &&dataSProgram) {
  createShaderProgram(dataSProgram);
  if (_colors.size() != _vertecies.size()) {
    colors =
        std::vector<glm::vec3>(_vertecies.size(), glm::vec3(1.f, 1.f, 1.f));
    setVBOData(&_vertecies, nullptr);
  } else
    setVBOData(&_vertecies, &_colors);
}
void mesh::setVBOData(const std::vector<glm::vec3> *_vertecies,
                      const std::vector<glm::vec3> *_colors) {
  if (_vertecies)
    vertecies = *_vertecies;
  if (_colors)
    colors = *_colors;

  initVAO();
}
void mesh::setColors(const std::vector<glm::vec3> &colors) {
  setVBOData(nullptr, &colors);
}
void mesh::setColor(glm::vec3 color) {
  std::vector<glm::vec3> nColors(vertecies.size(), color);
  setVBOData(nullptr, &nColors);
}
glm::vec3 mesh::getVertex(
    const std::vector<glm::vec3>                                 &vertecies,
    std::function<bool(const glm::vec3 &v1, const glm::vec3 &v2)> f) {
  glm::vec3 tmp = vertecies[0];
  for (int i = 1; i < vertecies.size(); ++i)
    if (!f(tmp, vertecies[i]))
      tmp = vertecies[i];
  return tmp;
}

void mesh::draw(const glm::mat4 &projection, const glm::mat4 &view,
                GLenum mode) {
  if (sProgram == nullptr) {
    std::cout << "The shader program is not initialized!" << std::endl;
    return;
  }

  attribObject.bind();

  attribObject.enable(mData.layoutVec3Vertex);
  attribObject.enable(mData.layoutVec3Color);

  sProgram->use();

  glm::mat4 VP = projection * view;

  glUniformMatrix4fv(mData.locationUniformVP, 1, GL_FALSE, &VP[0][0]);
  glUniformMatrix4fv(mData.locationUniformM, 1, GL_FALSE, &model[0][0]);
  if (_initEBO) {
    bufferElementsIndex.bind();
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, 0);
    bufferElementsIndex.unbind();
  } else
    glDrawArrays(mode, 0, vertecies.size());

  attribObject.disable(mData.layoutVec3Color);
  attribObject.disable(mData.layoutVec3Vertex);

  attribObject.unbind();
}
