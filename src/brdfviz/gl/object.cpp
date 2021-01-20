/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "object.h"
#include "vertexbufferobject.h"
#include "objecttransformation.h"
#include "shaders/diffuseshader.h"


int Object::IDcounter = 0;
std::map<int, Object *> Object::objects = std::map<int, Object *>();

Object::Object(const std::shared_ptr<VertexBufferObject> &innerObject,
               const std::shared_ptr<Shader> &shader,
               const std::shared_ptr<ObjectTransformation> &transformation,
               const std::shared_ptr<Material> &material) :
    visible(true),
    innerObject(/*innerObject == nullptr ? VertexBufferObject::cube : */innerObject),
    ID(IDcounter++),
    shader(shader == nullptr ? std::make_shared<DiffuseShader>() : shader),
    material(material == nullptr ? std::make_shared<Material>() : material),
    hasMtl(true),
    transformation(transformation == nullptr ?
                   std::make_shared<ObjectTransformation>(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.f), glm::vec3(0.0f)) :
                   transformation) {
  objects.insert_or_assign(ID, this);
}

void Object::draw(const bool geometry) {
  if (!visible) return;
//  if (this->transformation->isMoving()) {
//    this->transformation->move();
//  }
  //glStencilFunc(GL_ALWAYS, ID, 0xFF);
  shader->setModelMatrix(transformation->getModelMatrix());
  shader->use(material);
  
  if (geometry)
    this->innerObject->drawLines();
  else
    this->innerObject->draw();
}

void Object::setShader(const std::shared_ptr<Shader> &shader) {
  this->shader = shader;
}

void Object::setVisible(bool visible) {
  this->visible = visible;
}

bool Object::isVisible() {
  return visible;
}

bool Object::hasMaterial() {
  return hasMtl;
}

void Object::addMaterial(const std::shared_ptr<Material> &material) {
  this->material = material;
  hasMtl = true;
}

void Object::setPosition(glm::vec3 position) {
  this->transformation->setPosition(position);
}

int Object::getID() {
  return ID;
}

void Object::addName(const std::string &name) {
  this->objectName = name;
}

Object *Object::getObjectByID(int ID) {
  return objects.at(ID);
}
