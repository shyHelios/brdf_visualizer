/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "transformation.h"
#include <GLFW/glfw3.h>

Transformation::Transformation(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
  this->position = position;
  this->scale = scale;
  this->rotation = rotation;
  
  translationMat = glm::translate(glm::mat4(1.0f), position);
  rotationMat = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  rotationMat *= glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  rotationMat *= glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  scaleMat = glm::scale(glm::mat4(1.0f), scale);
  
  lastTime = float(glfwGetTime());
  movementSpeed = 1;
  rotationSpeed = 0.01f;
  
  posChanged = false;

//  hasCrv = false;
//  moving = false;

}

Transformation::~Transformation() {
  printf("Transformation destructor called\n");
}

glm::vec3 Transformation::getPosition() {
  return position;
}

void Transformation::setPosition(glm::vec3 position) {
  this->position = position;
  this->posChanged = true;
}

//float Transformation::getMovementSpeed() {
//  return movementSpeed;
//}
//
//void Transformation::setMovementSpeed(float movementSpeed) {
//  this->movementSpeed = movementSpeed;
//}
//
//void Transformation::addCurve(Curve *curve) {
//  this->curve = curve;
//  hasCrv = true;
//}
//
//void Transformation::removeCurve() {
//  delete (this->curve);
//  this->curve = NULL;
//  hasCrv = false;
//}
//
//bool Transformation::hasCurve() {
//  return hasCrv;
//}
//
//void Transformation::setMoving(bool moving) {
//  this->moving = moving;
//}
//
//bool Transformation::isMoving() {
//  return moving;
//}
//
//void Transformation::move() {
//  if (hasCrv && moving) {
//    curve->move(this->movementSpeed);
//  }
//}