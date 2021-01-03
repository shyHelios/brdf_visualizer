/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "openglscene.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "object.h"
#include "vertexbufferobject.h"

OpenGLScene::OpenGLScene() {
  this->hasSkBox = false;
}

OpenGLScene::~OpenGLScene() {
}

void OpenGLScene::addDefShader(Shader *shader) {
  this->defaultShader = shader;
}

void OpenGLScene::addObject(Object *o) {
  objects.emplace_back(o);
}

void OpenGLScene::addObject(std::vector<Object *> o) {
  objects.insert(objects.end(), o.begin(), o.end());
}

void OpenGLScene::render(const bool geometry) {
  if (this->camera == NULL || lights.size() < 1) return;
  this->camera->update();
  
  for (Light *l : lights) {
    l->update();
  }
  
  if (hasSkybox()) {
    glDisable(GL_DEPTH_TEST);
    for (Object *o : SkyBox) {
      o->setPosition(camera->getPosition());
      o->draw();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }
  if (lightObjects.size() > 0) {
    int i = 0;
    for (Object *o : lightObjects) {
      o->setShader(this->defaultShader);
      o->setPosition(lights.at(i)->getLigthInfo().position);
      o->draw();
      i++;
    }
  }
  if (objects.size() > 0) {
    for (Object *o : objects) {
      glStencilFunc(GL_ALWAYS, o->getID(), 0xFF);
      o->draw(geometry);
    }
  }
}

void OpenGLScene::addCamera(Camera *camera) {
  this->camera = camera;
}

Camera *OpenGLScene::getCamera() {
  return camera;
}

std::vector<Light *> OpenGLScene::getLights() {
  return this->lights;
}

void OpenGLScene::addLight(Light *light) {
  lights.emplace_back(light);
  
  Object *lightObject = new Object(VertexBufferObject::cube, this->defaultShader);
  Material *mtl = new Material();
  mtl->ambient_ = Color3f({light->getLigthInfo().ambient.x, light->getLigthInfo().ambient.y, light->getLigthInfo().ambient.z});
  mtl->diffuse_ = Color3f({light->getLigthInfo().diffuse.x, light->getLigthInfo().diffuse.y, light->getLigthInfo().diffuse.z});
  mtl->specular_ = Color3f({light->getLigthInfo().specular.x, light->getLigthInfo().specular.y, light->getLigthInfo().specular.z});
  lightObject->addMaterial(mtl);
  this->lightObjects.emplace_back(lightObject);
}

void OpenGLScene::addLight(std::vector<Light *> lights) {
  spdlog::warn("[OpenGLScene] AddLight(vector<Light*>) Not implemented yet \n");
}

bool OpenGLScene::hasSkybox() {
  return hasSkBox;
}

void OpenGLScene::addSkybox(Object *skybox) {
  hasSkBox = true;
  this->SkyBox.emplace_back(skybox);
}

void OpenGLScene::addSkybox(std::vector<Object *> skybox) {
  hasSkBox = true;
  this->SkyBox.insert(this->SkyBox.end(), skybox.begin(), skybox.end());
}

const std::vector<Object *> &OpenGLScene::getSkyBox() const {
  return SkyBox;
}
