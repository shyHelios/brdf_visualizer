/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SPHERE_H
#define SPHERE_H


#include <glm/vec3.hpp>

class Material;

#define RTC_COMPUTE_GEOMETRY_ID ((unsigned int)-2)

struct RTCRayHitIor;

class Sphere {
public:
  Sphere();
  
  Sphere(const glm::vec3 &worldPos, float radius = 1, const std::shared_ptr<Material> &material = nullptr);
  
  bool intersect(RTCRayHitIor &ray);

//private:
  glm::vec3 worldPos_;
  float radius_;
  
  std::shared_ptr<Material> material;
  
  glm::vec3 getNormal(const RTCRayHitIor &ray) const;
};


#endif //SPHERE_H
