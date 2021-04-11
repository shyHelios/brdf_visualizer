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


bool MathScene::occlude(RTCRay &ray) const {
  bool ret = false;
  glm::vec3 rayOrigin(ray.org_x, ray.org_y, ray.org_z);
  
  int sphereIndex = 0;
  for (const auto &sphere : spheres) {
    RTCRay localRay = ray;
    if (sphere->occlude(localRay)) {
      if (localRay.tfar < ray.tfar) {
        ray = localRay;
        ret = true;
      }
    }
    ++sphereIndex;
  }
  return ret;
}

glm::vec3 MathScene::getNormal(const RTCRayHitIor &ray) const {
  assert(ray.mathObjIndex >= 0 && ray.mathObjIndex < spheres.size());
  return spheres.at(ray.mathObjIndex)->getNormal(ray);
}

std::shared_ptr<Material> &MathScene::getMaterial(const RTCRayHitIor &ray) const {
  assert(ray.mathObjIndex >= 0 && ray.mathObjIndex < spheres.size());
  return spheres.at(ray.mathObjIndex)->material;
}

void MathScene::clearScene() {
  spheres.clear();
}

void MathScene::initDefaultScene(std::shared_ptr<Material> material) {
  if (material == nullptr) {
    material = std::make_shared<Material>();
    material->diffuse_.data = {0.1, 0.5, 0.9};
  }
  clearScene();
  auto sphere = std::make_unique<Sphere>(glm::vec3(0, 0, 0), 1.0f, material);
  spheres.emplace_back(std::move(sphere));
}

