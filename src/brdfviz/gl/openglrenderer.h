/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "renderer.h"

class OpenGLScene;

class OpenGLRenderer : public Renderer {
public:
  virtual void render(const bool geometry = false) override;
  
  unsigned int getCurrentSceneIndex() const;
  
  void setCurrentSceneIndex(unsigned int index);
  
  void addScene(const std::shared_ptr<OpenGLScene> &scene);
  
  void addScene(const std::shared_ptr<OpenGLScene> &, bool setThisScene);
  
  std::shared_ptr<OpenGLScene> &getCurrentScene();

private:
  
  unsigned int currentSceneIndex;
  std::vector<std::shared_ptr<OpenGLScene>> scenes;
};


#endif //OPENGLRENDERER_H
