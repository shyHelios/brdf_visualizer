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

class VertexBufferObject;

class OpenGLScene {
public:
  OpenGLScene();
  
  void addDefShader(const std::shared_ptr<Shader> &shader);
  
  void addObject(const std::shared_ptr<Object> &o);
  
  void addObject(const std::vector<std::shared_ptr<Object>> &o);
  
  void render(const bool geometry = false);
  
  void addCamera(const std::shared_ptr<Camera> &camera);
  
  std::shared_ptr<Camera> &getCamera();
  
  std::vector<std::shared_ptr<Light>> getLights();
  
  void addLight(const std::shared_ptr<Light> &light, const std::shared_ptr<VertexBufferObject> &lightVBO);
  
  bool hasSkybox();
  
  void addSkybox(const std::shared_ptr<Object> &skybox);
  
  void addSkybox(const std::vector<std::shared_ptr<Object>> &skybox);
  
  const std::vector<std::shared_ptr<Object>> &getSkyBox() const;

private:
  std::vector<std::shared_ptr<Object>> lightObjects;
  bool hasSkBox;
  std::vector<glm::vec4> materials;
  std::shared_ptr<Camera> camera;
  std::vector<std::shared_ptr<Object>> SkyBox;
  
  std::vector<std::shared_ptr<Object>> objects;
  std::shared_ptr<Shader> defaultShader;
  std::vector<std::shared_ptr<Light>> lights;
};


#endif //OPENGLSCENE_H
