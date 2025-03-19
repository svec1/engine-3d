#include <mesh.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

mesh::mesh(const std::vector<glm::vec3> &_vertecies,
           const std::vector<glm::vec3> &_colors,
           const dataShaderProgram      &dataSProgram)
    : vertecies(_vertecies), colors(_colors), bufferVertecies(GL_ARRAY_BUFFER),
      bufferColors(GL_ARRAY_BUFFER) {
  createShaderProgram(dataSProgram);
  initVAO();
}

void mesh::createShaderProgram(const dataShaderProgram &dataSProgram) {
  if (dataSProgram.vertexShader.empty()) {
    sProgram = dataSProgram.sProgram;
  } else {
    sProgram = std::shared_ptr<programShader>(new programShader);

    sProgram->createShader(GL_VERTEX_SHADER, dataSProgram.vertexShader);
    sProgram->createShader(GL_FRAGMENT_SHADER, dataSProgram.fragmentShader);
    sProgram->link();
  }

  mData = dataSProgram.mData;
  mData.locationUniformMVP = sProgram->getLocUniform(mData.nameUniformMVP);
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
void mesh::setPos(glm::vec3 pos) {
  model = glm::translate(glm::mat4(1.f), pos);
}
glm::vec3 mesh::getPos() { return model[3]; }

std::shared_ptr<programShader> mesh::getProgramShader() { return sProgram; }

void mesh::move(glm::vec3 nPos) { model = glm::translate(model, nPos); }
void mesh::rotate(float angle) {
  model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1.f, 0));
}
void mesh::scale(glm::vec3 scale) { model = glm::scale(model, scale); }

void mesh::setData(const std::vector<glm::vec3> &_vertecies,
                   const std::vector<glm::vec3> &_colors,
                   const dataShaderProgram      &dataSProgram) {
  sProgram->dump();
  vertecies = _vertecies;
  colors = _colors;
  createShaderProgram(dataSProgram);
  initVAO();
}
void mesh::draw(const glm::mat4 &projection, const glm::mat4 &view) {
  if (sProgram == nullptr) {
    std::cout << "The shader program is not initialized!" << std::endl;
    return;
  }

  attribObject.bind();

  attribObject.enable(mData.layoutVec3Vertex);
  attribObject.enable(mData.layoutVec3Color);

  sProgram->use();

  glm::mat4 MVP = projection * view * model;

  glUniformMatrix4fv(mData.locationUniformMVP, 1, GL_FALSE, &MVP[0][0]);
  glDrawArrays(GL_TRIANGLES, 0, vertecies.size());

  attribObject.disable(mData.layoutVec3Color);
  attribObject.disable(mData.layoutVec3Vertex);

  attribObject.unbind();
}
