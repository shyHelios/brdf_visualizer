/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
//#include "application.h"
//#include "controller.h"
#include "camera.h"
#include "shader.h"


void Camera::update() {
  CameraInfo cameraInfo{};
  cameraInfo.viewMatrix = this->getViewMatrix();
  
  cameraInfo.cameraPosition = transformation->getPosition(target);
  cameraInfo.projectionMatrix = getProjectionMatrix();
  
  
  if (this->camInfo.cameraPosition != cameraInfo.cameraPosition ||
      this->camInfo.projectionMatrix != cameraInfo.projectionMatrix ||
      this->camInfo.viewMatrix != cameraInfo.viewMatrix) {
    this->camInfo = cameraInfo;
    camInfoChanged = true;
    notifyObservers();
  }
}

Camera::Camera() {
  cameraUp = glm::vec3(0.0f, 0.0f, -1.0f);
  angle = glm::vec3(0.f);
  transformation = std::make_shared<CameraTransformation>(glm::vec3(0.f, 0.f, 5.f));
//  transformation->setMovementSpeed(3.f);
  yaw = -90.0f;  // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
  pitch = 0.0f;
}

glm::mat4 Camera::getViewMatrix() {
  getTarget();
  glm::vec3 eye = transformation->getPosition(target);
  glm::mat4 viewMatrix;
  if (transformation->overrideTarget.second) {
    viewMatrix = glm::lookAt(eye, target, cameraUp);
  } else {
    viewMatrix = glm::lookAt(eye, eye + target, cameraUp);
  }
  
  return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const {
  return glm::perspective(glm::radians(45.0f), ratio, 0.1f, 100.0f);
}

void Camera::addShader(const std::shared_ptr<Shader> &shader) {
  shaders.emplace_back(shader);
}

glm::vec3 Camera::getPosition() {
  return transformation->getPosition(target);
}

void Camera::notifyObservers() {
  if (camInfoChanged) {
    for (std::weak_ptr<Shader> &s : shaders)
      if (auto shaderSharedPtr = s.lock()) {
        shaderSharedPtr->notify(this->camInfo);
      } else {
        spdlog::warn("[Camera] Shader ptr expired");
      }
    camInfoChanged = false;
  }
}

glm::vec3 Camera::getTarget() {
  return target;
}

float Camera::getRatio() const {
  return ratio;
}

void Camera::setRatio(float ratio) {
  this->ratio = ratio;
}
