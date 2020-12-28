/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "cameratransformation.h"
#include <pch.h>
//#include "Application.h"

CameraTransformation::CameraTransformation(glm::vec3 position) :
    Transformation(position,
                   glm::vec3(1.0f),
                   glm::vec3(0.0f)),
    overrideTarget(glm::vec3(0, 0, 0), false) {
}

glm::vec3 CameraTransformation::getPosition(glm::vec3 target) {
  //Keyboard controlling

//  if (hasCurve() && isMoving()) {
//    glm::vec3 oldPos = position;
//    position = this->curve->getPoint();
//
//    posChanged = oldPos != position;
//  } else {
  float currentTime = float(glfwGetTime());
  float deltaTime = float(currentTime - lastTime);
  lastTime = currentTime;
//  GLFWwindow *window = Application::getInstance()->getWindow()->getGlfwWindow();
  
  posChanged = false;

//  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
//    position += target * deltaTime * movementSpeed;
//    posChanged = true;
//  }
//  // Move backward
//  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
//    position -= target * deltaTime * movementSpeed;
//    posChanged = true;
//  }
//  // Strafe right
//  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
//    position += glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime * movementSpeed;
//    posChanged = true;
//  }
//  // Strafe left
//  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
//    position -= glm::cross(target, glm::vec3(0.0f, 1.0f, 0.0f)) * deltaTime * movementSpeed;
//    posChanged = true;
//  }
//  }
  
  return position;
}

CameraTransformation::~CameraTransformation() {
}