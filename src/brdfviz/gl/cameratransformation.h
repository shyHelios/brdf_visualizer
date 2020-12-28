/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef CAMERATRANSFORMATION_H
#define CAMERATRANSFORMATION_H

#include "transformation.h"

class CameraTransformation :
    public Transformation {
public:
  CameraTransformation(glm::vec3 position = glm::vec3(0.f));
  
  glm::vec3 getPosition(glm::vec3 target);
  
  ~CameraTransformation();
  
  //glm::mat4 getProjectionMatrix(glm::vec3 target);
  
  std::pair<glm::vec3, bool> overrideTarget;
};


#endif //CAMERATRANSFORMATION_H
