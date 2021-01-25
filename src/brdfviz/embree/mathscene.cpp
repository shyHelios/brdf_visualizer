/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "mathscene.h"
#include "sphere.h"
#include "rtcrayhitior.h"
#include <gl/material.h>

bool MathScene::intersect(RTCRayHitIor &ray) const {
  bool ret = false;
  glm::vec3 rayOrigin(ray.ray.org_x, ray.ray.org_y, ray.ray.org_z);
  
  int sphereIndex = 0;
  for (const auto &sphere : spheres) {
    RTCRayHitIor localRay = ray;
    
    //double distance = glm::l2Norm(sphere->worldPos_, rayOrigin);
    //if (distance < ray.ray.tfar) {
    if (sphere->intersect(localRay)) {
      localRay.mathObjIndex = sphereIndex;
      if (localRay.ray.tfar < ray.ray.tfar) {
        ray = localRay;
        ret = true;
      }
    }
    ++sphereIndex;
    //}
    
  }
  return ret;
}


glm::vec3 MathScene::getNormal(const RTCRayHitIor &ray) const {
  assert(ray.mathObjIndex >= 0 && ray.mathObjIndex < spheres.size());
  return spheres.at(ray.mathObjIndex)->getNormal(ray);
}

std::shared_ptr<Material> MathScene::getMaterial(const RTCRayHitIor &ray) const {
  assert(ray.mathObjIndex >= 0 && ray.mathObjIndex < spheres.size());
  return spheres.at(ray.mathObjIndex)->material;
}