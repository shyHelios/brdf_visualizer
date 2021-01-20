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

void OpenGLScene::addDefShader(const std::shared_ptr<Shader> &shader) {
  this->defaultShader = shader;
}

void OpenGLScene::addObject(const std::shared_ptr<Object> &o) {
  objects.emplace_back(o);
}

void OpenGLScene::addObject(const std::vector<std::shared_ptr<Object>> &o) {
  objects.insert(objects.end(), o.begin(), o.end());
}

void OpenGLScene::render(const bool geometry) {
  if (this->camera == NULL || lights.size() < 1) return;
  this->camera->update();
  
  for (auto l : lights) {
    l->update();
  }
  
  if (hasSkybox()) {
    glDisable(GL_DEPTH_TEST);
    for (const std::shared_ptr<Object> &o : SkyBox) {
      o->setPosition(camera->getPosition());
      o->draw();
    }
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  }
  if (lightObjects.size() > 0) {
    int i = 0;
    for (const std::shared_ptr<Object> &o : lightObjects) {
      o->setShader(this->defaultShader);
      o->setPosition(lights.at(i)->getLigthInfo().position);
      o->draw();
      i++;
    }
  }
  if (objects.size() > 0) {
    for (const std::shared_ptr<Object> &o : objects) {
      glStencilFunc(GL_ALWAYS, o->getID(), 0xFF);
      o->draw(geometry);
    }
  }
}

void OpenGLScene::addCamera(const std::shared_ptr<Camera> &camera) {
  this->camera = camera;
}

std::shared_ptr<Camera> &OpenGLScene::getCamera() {
  return camera;
}

std::vector<std::shared_ptr<Light>> OpenGLScene::getLights() {
  return this->lights;
}

void OpenGLScene::addLight(const std::shared_ptr<Light> &light, const std::shared_ptr<VertexBufferObject> &lightVBO) {
  lights.emplace_back(light);
  
  std::shared_ptr<Object> lightObject = std::make_shared<Object>(lightVBO, this->defaultShader);
  std::shared_ptr<Material> mtl = std::make_shared<Material>();
  const auto lInfo = light->getLigthInfo();
  mtl->ambient_ = Color3f({lInfo.ambient.x, lInfo.ambient.y, lInfo.ambient.z});
  mtl->diffuse_ = Color3f({lInfo.diffuse.x, lInfo.diffuse.y, lInfo.diffuse.z});
  mtl->specular_ = Color3f({lInfo.specular.x, lInfo.specular.y, lInfo.specular.z});
  lightObject->addMaterial(mtl);
  this->lightObjects.emplace_back(lightObject);
}

bool OpenGLScene::hasSkybox() {
  return hasSkBox;
}

void OpenGLScene::addSkybox(const std::shared_ptr<Object> &skybox) {
  hasSkBox = true;
  this->SkyBox.emplace_back(skybox);
}

void OpenGLScene::addSkybox(const std::vector<std::shared_ptr<Object>> &skybox) {
  hasSkBox = true;
  this->SkyBox.insert(this->SkyBox.end(), skybox.begin(), skybox.end());
}

const std::vector<std::shared_ptr<Object>> &OpenGLScene::getSkyBox() const {
  return SkyBox;
}
