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
  if (!_sProgramObjects || !_sProgramGrid)
    throw std::runtime_error("Invalid program shader");

  sProgram = _sProgramObjects;
  gr.init({_sProgramGrid});

  grLocObjectComponents =
      gr.getProgramShader()->getLocUniform("objectComponents");
  grLocGravityConstant =
      gr.getProgramShader()->getLocUniform("gravityConstant");
}
void universe::setGridVisible(bool _gridVisible) { gridVisible = _gridVisible; }
void universe::setTraceVisible(bool _traceVisible) {
  traceVisible = _traceVisible;
}

void universe::setGravityConstant(const float _gravityConstant) {
  gravityConstant = _gravityConstant;
}

std::shared_ptr<const physicObject>
universe::getObject(unsigned int index) const {
  if (index >= objects.size())
    return {};
  return objects[index];
}
bool        universe::getGridVisible() const { return gridVisible; }
bool        universe::getTraceVisible() const { return traceVisible; }
const float universe::getGravityConstant() const { return gravityConstant; }
std::size_t universe::getCountObjects() const { return objects.size(); }

void universe::createObject(float mass, float radius, glm::vec3 pos,
                            glm::vec3 speed) {
  objects.push_back(std::make_shared<physicObject>(pos, radius, mass));
  objects[objects.size() - 1]->init({sProgram});
  objects[objects.size() - 1]->traceObject.init({sProgram});

  objects[objects.size() - 1]->setColor(glm::vec4{
      1.f / (float)(std::rand() % 10), 1.f / (float)(std::rand() % 10),
      1.f / (float)(std::rand() % 10), 1.f});
  objects[objects.size() - 1]->setSpeed(speed);
}
void universe::deleteObject(unsigned int index) {
  if (index >= objects.size())
    return;
  objects.erase(objects.begin() + index);
}

void universe::deleteAllObjects() { objects.clear(); }

void universe::generate(std::size_t countPlanets, std::size_t minMass,
                        std::size_t maxMass, std::size_t minDistance) {
  deleteAllObjects();
  countPlanets = glm::clamp(countPlanets, (std::size_t)0, (std::size_t)1024);

  for (std::size_t i = 0; i < countPlanets; ++i) {
    std::size_t massObject =
        glm::clamp(std::rand() % 10000 * i + 1000, minMass, maxMass);
    createObject(massObject,
                 glm::clamp(rand() % 10 + i / (countPlanets + 1),
                            (std::size_t)1, (std::size_t)4 * countPlanets + 10),
                 {std::rand() % minDistance + i,
                  std::rand() % minDistance + i / 4,
                  std::rand() % minDistance + i},
                 {std::rand() % 100 / 100, std::rand() % 100 / 100,
                  std::rand() % 100 / 100});
  }
}

void universe::simulation() {
  for (unsigned int i = 0; i < objects.size(); ++i) {
    glm::vec3 tmpForce(0.f);

    const auto &object1 = objects[i];

    const glm::vec3 posObject1 = object1->getPos();
    const float     massObject1 = object1->getMass();

    for (unsigned int j = 0; j < objects.size(); ++j) {
      if (i == j)
        continue;

      const auto &object2 = objects[j];

      const glm::vec3 posObject2 = object2->getPos();
      const float     massObject2 = object2->getMass();

      const float r = std::max(glm::distance(posObject1, posObject2), 0.00001f);
      const glm::vec3 direction = glm::normalize(posObject2 - posObject1);

      const float scalar =
          gravityConstant * (massObject1 * massObject2) / pow(r, 2);

      const glm::vec3 tmpForce2 = direction * scalar;
      tmpForce += tmpForce2;

      object2->physicMove(tmpForce2 / massObject2 * -1.f);
    }
    object1->physicMove(tmpForce / massObject1);
  }
}
void universe::render(const glm::mat4 &projection, const glm::mat4 &view) {
  std::vector<glm::vec4> objectComponents;

  static float lastMassiveObjectY = 0.f;

  float massiveObject = 0.f;
  float massiveObjectY = 0.f;

  for (auto &object : objects) {
    object->draw(projection, view);
    if (traceVisible) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      object->traceObject.addPosition(object->getPos());
      object->traceObject.draw(projection, view, GL_LINES);
      glDisable(GL_BLEND);
    }

    if (gridVisible) {
      glm::vec3 objectPos = object->getPos();
      glm::vec4 components = glm::vec4(objectPos, object->getRadius());
      components.y = object->getMass();

      objectComponents.push_back(components);

      if (components.y > massiveObject) {
        massiveObjectY = objectPos.y;
        massiveObject = components.y;
      }
    }
  }

  if (gridVisible) {
    // Moving the grid
    {
      auto cameraPos = glm::inverse(view)[3];
      auto gridPos = gr.getPos();

      glm::vec3 maxVertexGrid = gr.getMaxTransformVertex(),
                minVertexGrid = gr.getMinTransformVertex();

      if (cameraPos.x + 1000 > maxVertexGrid.x)
        gr.move({0.05, 0, 0});
      else if (cameraPos.x - 1000 <= minVertexGrid.x)
        gr.move({-0.05, 0, 0});

      if (cameraPos.z + 1000 >= maxVertexGrid.z)
        gr.move({0, 0, 0.05});
      else if (cameraPos.z - 1000 < minVertexGrid.z)
        gr.move({0, 0, -0.05});

      gr.move({0, massiveObjectY - lastMassiveObjectY, 0});
      lastMassiveObjectY = massiveObjectY;
    }

    gr.getProgramShader()->use();

    glUniform1f(grLocGravityConstant, gravityConstant);
    glUniform4fv(grLocObjectComponents, objectComponents.size(),
                 glm::value_ptr(objectComponents[0]));

    gr.draw(projection, view, GL_LINES);
  }
}
float universe::calculateWorkSystem() {
  float currentW = 0.f;
  for (std::size_t i = 0; i < objects.size(); ++i)
    currentW +=
        objects[i]->getMass() * pow(glm::length(objects[i]->getSpeed()), 2) / 2;

  float lastWTmp = lastW;
  lastW = currentW;

  return currentW - lastWTmp;
}
