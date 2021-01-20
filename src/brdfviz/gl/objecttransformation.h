/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef OBJECTTRANSFORMATION_H
#define OBJECTTRANSFORMATION_H


#include "transformation.h"

class ObjectTransformation : public Transformation {
public:
  ObjectTransformation(glm::vec3 position = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f), glm::vec3 rotation = glm::vec3(0.f));
  
  glm::mat4 getModelMatrix();
  
  glm::vec3 rotationSpeed;
  
  bool scaleChanged;
  
};

#endif //OBJECTTRANSFORMATION_H
