/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef CAMERA_H
#define CAMERA_H



//Include GLM
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include "cameratransformation.h"
#include "camerainfo.h"
#include <vector>

class Shader;

class Camera {
public:
  void update();
  
  Camera();
  
  glm::mat4 getViewMatrix();
  
  glm::mat4 getProjectionMatrix();
  
  void addShader(const std::shared_ptr<Shader> &shader);
  
  glm::vec3 getPosition();
  
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);

//private:
  void notifyObservers();
  
  CameraInfo camInfo{};
  bool camInfoChanged{};
  
  glm::vec3 getTarget();
  
  glm::vec3 angle{};
  glm::vec3 cameraUp{};
  glm::vec3 target{};
  glm::mat4 projectionMatrix{};
  std::shared_ptr<CameraTransformation> transformation;
  std::vector<std::weak_ptr<Shader>> shaders;
  
  float yaw;
  float pitch;
  float ratio = 1.0;
  
  float getRatio() const;
  
  void setRatio(float ratio);
  
  bool firstMouse{};
  double lastX{};
  double lastY{};
};


#endif //CAMERA_H
