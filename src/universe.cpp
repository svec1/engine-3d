#include <universe.hpp>

#include <cmath>
#include <cstdlib>

#include <glm/gtc/type_ptr.hpp>

universe::universe(const float _gravityConstant)
    : gravityConstant(_gravityConstant) {
  gr.setPos(glm::vec3{-250.f, 0, -250.f});
  gr.scale(glm::vec3{5000.f, 1.f, 5000.f});
}
void universe::setProgramsShader(
    std::shared_ptr<programShader> _sProgramObjects,
    std::shared_ptr<programShader> _sProgramGrid) {
  sProgram = _sProgramObjects;
  gr.init({_sProgramGrid});
}
glm::vec3 universe::getPosObject(unsigned int index) {
  if (index >= objects.size())
    return glm::vec3{0.f};
  return objects[index]->getPos();
}

void universe::createObject(float mass, float radius, glm::vec3 pos,
                            glm::vec3 speed) {
  objects.push_back(std::make_unique<physicObject>(pos, radius, mass));
  objects[objects.size() - 1]->init({sProgram});

  objects[objects.size() - 1]->setColor(glm::vec3{
      1.f / (float)(std::rand() % 10), 1.f / (float)(std::rand() % 10),
      1.f / (float)(std::rand() % 10)});
  objects[objects.size() - 1]->setSpeed(speed);
}
void universe::deleteObject(unsigned int index) {
  if (index >= objects.size())
    return;
  objects.erase(objects.begin() + index);
}

void universe::simulation() {
  for (unsigned int i = 0; i < objects.size(); ++i) {
    glm::vec3 tmpForce(0.f);

    for (unsigned int j = 0; j < objects.size(); ++j) {
      if (i == j)
        continue;

      const glm::vec3 posObj1 = objects[i]->getPos();
      const glm::vec3 posObj2 = objects[j]->getPos();

      const float     r = std::max(glm::distance(posObj1, posObj2), 0.00001f);
      const glm::vec3 direction = glm::normalize(posObj2 - posObj1);

      const float scalar = gravityConstant *
                           (objects[i]->getMass() * objects[j]->getMass()) /
                           pow(r, 2);

      glm::vec3 tmpForce2 =
          /* glm::vec3(scalar, 0, scalar) * direction */ direction * scalar;
      tmpForce += tmpForce2;

      objects[j]->physicMove(tmpForce2 / objects[j]->getMass() * -1.f);
    }
    objects[i]->physicMove(tmpForce / objects[i]->getMass());
  }
}
void universe::render(const glm::mat4 &projection, const glm::mat4 &view) {
  GLuint locObjectComponents =
      gr.getProgramShader()->getLocUniform("objectComponents");

  std::vector<glm::vec4> objectComponents;
  for (auto &object : objects) {
    object->draw(projection, view);

    glm::vec4 components = glm::vec4(object->getPos(), object->getRadius());
    components.y = object->getMass();

    objectComponents.push_back(components);
  }
  gr.getProgramShader()->use();
  glUniform1f(gr.getProgramShader()->getLocUniform("gravityConstant"),
              gravityConstant);
  glUniform4fv(locObjectComponents, objectComponents.size(),
               glm::value_ptr(objectComponents[0]));
  gr.draw(projection, view, GL_LINES);
}

