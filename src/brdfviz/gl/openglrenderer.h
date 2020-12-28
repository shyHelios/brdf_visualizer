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
  virtual void render() override;
  
  unsigned int getCurrentSceneIndex() const;
  
  void setCurrentSceneIndex(unsigned int index);
  
  void addScene(OpenGLScene *scene);
  
  void addScene(OpenGLScene *scene, bool setThisScene);
  
  OpenGLScene *getCurrentScene();

private:
  
  unsigned int currentSceneIndex;
  std::vector<OpenGLScene *> scenes;
};


#endif //OPENGLRENDERER_H
