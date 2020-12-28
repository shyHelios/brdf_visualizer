/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "objecttransformation.h"
//#include "Application.h"

ObjectTransformation::ObjectTransformation(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) : Transformation(
    position, scale, rotation) {
  scaleChanged = false;
  rotationSpeed = glm::vec3(0, 0, 0);
}

ObjectTransformation::~ObjectTransformation() {
}

glm::mat4 ObjectTransformation::getModelMatrix() {
//  if (hasCrv && moving) {
//    glm::vec3 oldPos = position;
//    position = this->curve->getPoint();
//
//    posChanged = oldPos != position;
//  }
  
  if (posChanged) {
    translationMat = glm::translate(glm::mat4(1.0f), position);
    posChanged = false;
  }
  rotation += rotationSpeed;
//  rotationMat = glm::mat4(1.0f);
//  rotationMat = glm::rotate(glm::mat4(1.0f), rotation += rotationSpeed, glm::vec3(1.0f, 1.0f, 1.0f));
//  rotation.x += 0.01;
  rotationMat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  rotationMat *= glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  rotationMat *= glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  
  if (scaleChanged) {
    scaleMat = glm::scale(glm::mat4(1.0f), scale);
    scaleChanged = false;
  }
  return translationMat * rotationMat * scaleMat;
}