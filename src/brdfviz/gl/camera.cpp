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
  CameraInfo camInfo;
  camInfo.viewMatrix = this->getViewMatrix();


//  if (transformation->isMoving() && transformation->hasCurve()) {
//    //camInfo.viewMatrix = glm::lookAt(camInfo.cameraPosition, glm::vec3(1.f), cameraUp);
//    camInfo.cameraPosition = transformation->getPosition(target);
//    transformation->move();
//  } else {
  camInfo.cameraPosition = transformation->getPosition(target);
//  }
  
  camInfo.projectionMatrix = getProjectionMatrix();
  
  
  if (this->camInfo.cameraPosition != camInfo.cameraPosition ||
      this->camInfo.projectionMatrix != camInfo.projectionMatrix ||
      this->camInfo.viewMatrix != camInfo.viewMatrix) {
    this->camInfo = camInfo;
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

glm::mat4 Camera::getProjectionMatrix() {
  //TODO TEMPORARY
  float ratio = 1.0f;
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
//  if (transformation->overrideTarget.second) {
//    target = transformation->overrideTarget.first;
//  } else {
//    glm::vec2 offset = Controller::getInstance()->getCursorPos();
//    double xpos = offset.x;
//    double ypos = offset.y;
//    if (firstMouse) {
//      lastX = xpos;
//      lastY = ypos;
//      firstMouse = false;
//    }
//
//    double xoffset = xpos - lastX;
//    double yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    double sensitivity = 0.1;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch -= yoffset;
//
//    if (pitch > 89.0f)
//      pitch = 89.0f;
//    if (pitch < -89.0f)
//      pitch = -89.0f;
//
//    glm::vec3 direction;
//    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    direction.y = sin(glm::radians(pitch));
//    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    target = glm::normalize(direction);
//  }
  
  return target;
}

void Camera::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  
  double xoffset = xpos - lastX;
  double yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
  
  double sensitivity = 0.05;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  
  yaw += xoffset;
  pitch += yoffset;
  
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  target = glm::normalize(direction);
}
