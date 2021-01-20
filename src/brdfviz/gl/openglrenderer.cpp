/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "openglrenderer.h"
#include "openglscene.h"

void OpenGLRenderer::render(const bool geometry) {
  
  glCall(glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f)); // state setting function
  glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)); // state using function
  glCall(glEnable(GL_DEPTH_TEST));
  glCall(glDepthFunc(GL_LESS));
  glCall(glEnable(GL_MULTISAMPLE));
  if (!scenes.empty())
    scenes.at(currentSceneIndex)->render(geometry);
}

unsigned int OpenGLRenderer::getCurrentSceneIndex() const {
  return this->currentSceneIndex;
}

void OpenGLRenderer::setCurrentSceneIndex(unsigned int index) {
  if (index < scenes.size())
    this->currentSceneIndex = index;
}

void OpenGLRenderer::addScene(const std::shared_ptr<OpenGLScene> &scene) {
  scenes.emplace_back(scene);
  
}

void OpenGLRenderer::addScene(const std::shared_ptr<OpenGLScene> &scene, bool setThisScene) {
  int index = scenes.size();
  scenes.emplace_back(scene);
  if (setThisScene)
    setCurrentSceneIndex(index);
}

std::shared_ptr<OpenGLScene> &OpenGLRenderer::getCurrentScene() {
  return scenes.at(currentSceneIndex);
}

