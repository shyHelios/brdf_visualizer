/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

class Shader;

class Object;

class Light;

class Camera;

class OpenGLScene {
public:
  OpenGLScene();
  
  ~OpenGLScene();
  
  void addDefShader(Shader *shader);
  
  void addObject(Object *o);
  
  void addObject(std::vector<Object *> o);
  
  void render();
  
  void addCamera(Camera *camera);
  
  Camera *getCamera();
  
  std::vector<Light *> getLights();
  
  void addLight(Light *light);
  
  void addLight(std::vector<Light *> lights);
  
  bool hasSkybox();
  
  void addSkybox(Object *skybox);
  
  void addSkybox(std::vector<Object *> skybox);
  
  const std::vector<Object *> &getSkyBox() const;

//  ShadowMap *shadowMap_{nullptr};
private:
  std::vector<Object *> lightObjects;
  bool hasSkBox;
  std::vector<glm::vec4> materials;
  Camera *camera;
  std::vector<Object *> SkyBox;
  
  std::vector<Object *> objects;
  Shader *defaultShader;
  Object *cdObjects;
  std::vector<Light *> lights;
};


#endif //OPENGLSCENE_H
