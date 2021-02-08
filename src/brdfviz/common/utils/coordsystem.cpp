/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "coordsystem.h"
#include "math.h"

void coordSystem(const glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent) {
  glm::vec3 c1 = cross(normal, UP);
  glm::vec3 c2 = cross(normal, TANG);
  if (glm::length(c1) > glm::length(c2)) {
    tangent = c1;
  } else {
    tangent = c2;
  }
  tangent = glm::normalize(tangent);
  bitangent = cross(normal, tangent);
  bitangent = glm::normalize(bitangent);
}

glm::mat3 coordTransformMatrix(const glm::vec3 &normal) {
  const glm::vec3 f/*romCoordSpace*/ = UP;
  const glm::vec3 t/*oCoordSpace*/ = normal;
  const glm::vec3 v = glm::cross(f, t);
//  const glm::vec3 u = v / glm::l2Norm(v);
  
  const float c = glm::dot(f, t);
  const float h = (1.f - c) / (1.f - sqr(c));
  if (c == 1) {
    return glm::mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
  }
  const glm::mat3 rotationMatrix = {
      c + h * sqr(v.x), h * v.x * v.y - v.z, h * v.x * v.z + v.y,
      h * v.x * v.y + v.z, c + h * sqr(v.y), h * v.y * v.z - v.x,
      h * v.x * v.z - v.y, h * v.y * v.z + v.x, c + h * sqr(v.z)
  };
  
  return rotationMatrix;

//  const glm::vec3 v1 = normal;
//  const glm::vec3 v2 = UP;
//
//  float cosTheta = glm::dot(v1, v2);
//  float sinTheta = glm::l2Norm(glm::cross(v1, v2));
//
////  return glm::mat3({cosTheta, -sinTheta, 0},
////                   {sinTheta, cosTheta, 0},
////                   {0, 0, 1});
//  return glm::mat3({cosTheta, sinTheta, 0},
//                   {-sinTheta, cosTheta, 0},
//                   {0, 0, 1});
  //  glm::vec3 norm = normal;
//  norm = glm::normalize(norm);
//
//  glm::vec3 tangent;
//
//  glm::vec3 c1 = cross(UP, norm);
//  glm::vec3 c2 = cross(TANG, norm);
//  if (glm::length(c1) >= 0) {
//    tangent = c1;
//  } else {
//    tangent = c2;
//  }
//  tangent = glm::normalize(tangent);
//
//  glm::vec3 bitangent = glm::cross(norm, tangent);
//  bitangent = glm::normalize(bitangent);
//
//  return glm::mat3(tangent, bitangent, norm);


}

glm::vec3 localToWorld(const glm::vec3 &sample, const glm::vec3 &normal) {
//  glm::vec3 tangent, bitangent;
//  coordSystem(normal, tangent, bitangent);
//  return glm::vec3(sample.x * bitangent.x + sample.y * normal.x + sample.z * tangent.x,
//                   sample.x * bitangent.y + sample.y * normal.y + sample.z * tangent.y,
//                   sample.x * bitangent.z + sample.y * normal.z + sample.z * tangent.z);
  auto transformationMatrix = coordTransformMatrix(normal);
  return sample * transformationMatrix;
}