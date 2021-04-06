/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef MATHSCENE_H
#define MATHSCENE_H


#include "rtcrayhitior.h"
#include "sphere.h"
#include "gl/material.h"

class MathScene {
public:
  bool intersect(RTCRayHitIor &ray) const;
  
  bool occlude(RTCRay &ray) const;
  
  glm::vec3 getNormal(const RTCRayHitIor &ray) const;
  
  std::shared_ptr<Material> getMaterial(const RTCRayHitIor &ray) const;
  
  std::vector<std::unique_ptr<Sphere>> spheres;
  
  void clearScene();
  
  void initDefaultScene();
};


#endif //MATHSCENE_H
