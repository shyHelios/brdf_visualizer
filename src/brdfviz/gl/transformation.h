/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
//#include "Curve.h"

class Transformation {
public:
  Transformation(glm::vec3 position = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f), glm::vec3 rotation = glm::vec3(0.f));
  
  glm::vec3 getPosition();
  
  void setPosition(glm::vec3 position);

//  float getMovementSpeed();

//  void setMovementSpeed(float movementSpeed);

//  void addCurve(Curve *curve);

//  void removeCurve();

//  bool hasCurve();

//  void setMoving(bool moving);

//  bool isMoving();

//  void move();

protected:
  float lastTime;
  float movementSpeed;
  float rotationSpeed;
  
  bool posChanged;
  glm::mat4 translationMat;
  glm::mat4 rotationMat;
  glm::mat4 scaleMat;
  
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;

//  bool hasCrv;
//  bool moving;
//  Curve *curve;
};


#endif //TRANSFORMATION_H
