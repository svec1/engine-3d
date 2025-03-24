#include <universe.hpp>

#include <cmath>
#include <cstdlib>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>

static const char *vertex_shader_text =
    "#version 460\n"
    "layout (location = 0) in vec3 vPos;\n"
    "layout (location = 1) in vec3 vClr;\n"
    "out vec3 VFragColor;\n"
    "uniform mat4 M;\n"
    "uniform mat4 VP;\n"
    "uniform vec4 objectComponents[256];"
    "void main()\n"
    "{\n"
    "    vec4 tmpPos = M * vec4(vPos, 1.0);\n"
    "    for(int i = 0; i < 256; ++i){\n"
    "       float distance = (pow(objectComponents[i].x-tmpPos.x, "
    "2)+pow(objectComponents[i].z-tmpPos.z, 2))/(objectComponents[i].w*2);\n"
    "       distance = max(distance, 7);\n"
    "       tmpPos.y -= objectComponents[i].y/distance;\n"
    "    }\n"
    "    vec4 nColor = normalize(tmpPos)/3;"
    //"    VFragColor = vClr;"
    "    VFragColor = vec3(vClr.x + nColor.y, vClr.y + nColor.y, vClr.z+ "
    "nColor.y);\n"
    "    gl_Position = VP * tmpPos;\n"
    "}";

static const char *fragment_shader_text = "#version 460\n"
                                          "in vec3 VFragColor;\n"
                                          "out vec3 FragColor;\n"
                                          "void main()\n"
                                          "{\n"
                                          "    FragColor = VFragColor;\n"
                                          "}\n";

universe::universe(const float _gravityConstant)
    : gravityConstant(_gravityConstant) {
  gr.setPos(glm::vec3{-250.f, 0, -250.f});
  gr.scale(glm::vec3{1000.f, 1.f, 1000.f});
}
void universe::setProgramShader(std::shared_ptr<programShader> _sProgram) {
  sProgram = _sProgram;
  gr.init({vertex_shader_text, fragment_shader_text});
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

      const float     r = glm::distance(posObj1, posObj2);
      const glm::vec3 direction = glm::normalize(posObj2 - posObj1);

      const float scalar = gravityConstant *
                           (objects[i]->getMass() * objects[j]->getMass()) /
                           pow(r, 2);

      glm::vec3 tmpForce2 = glm::vec3(scalar, 0, scalar) * direction;
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
  glUniform4fv(locObjectComponents, objectComponents.size(),
               glm::value_ptr(objectComponents[0]));
  gr.draw(projection, view, GL_LINES);
}

