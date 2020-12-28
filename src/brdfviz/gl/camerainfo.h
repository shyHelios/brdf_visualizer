/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::vec3

struct CameraInfo {
  glm::vec3 cameraPosition;
  glm::mat4 viewMatrix;
  glm::mat4 projectionMatrix;
};

#endif //CAMERAINFO_H
