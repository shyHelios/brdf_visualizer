/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/math.h>
#include "sphere.h"
#include "rtcrayhitior.h"

#define RAY_OFFSET 0.5

bool Sphere::intersect(RTCRayHitIor &ray) {
  
  double rayOriginX = ray.ray.org_x;
  double rayOriginY = ray.ray.org_y;
  double rayOriginZ = ray.ray.org_z;
  
  double rayDirectionX = ray.ray.dir_x;
  double rayDirectionY = ray.ray.dir_y;
  double rayDIrectionZ = ray.ray.dir_z;
  
  double spherePosX = worldPos_.x;
  double spherePosY = worldPos_.y;
  double spherePosZ = worldPos_.z;
  
  double a = sqr(ray.ray.dir_x) + sqr(ray.ray.dir_y) + sqr(ray.ray.dir_z);
  double b = 2. * rayDirectionX * (rayOriginX - spherePosX) + 2 * rayDirectionY * (rayOriginY - spherePosY) +
             2 * rayDIrectionZ * (rayOriginZ - spherePosZ);
  double c = sqr(rayOriginX - spherePosX) + sqr(rayOriginY - spherePosY) + sqr(rayOriginZ - spherePosZ) - sqr(radius_);
  
  double d = sqr(b) - 4. * a * c;
  
  float tfar = 0;
  
  if (d < 0) {
    //no intersect point
    ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    return false;
  } else if (d == 0.) {
    //glm::vec3 intersectPoint(0, 0, 0);
    
    tfar = -b / 2. * a;
    //one intersect point
  } else {
//    glm::vec3 intersectPoint1(0, 0, 0);
//    glm::vec3 intersectPoint2(0, 0, 0);
    
    double sqrtd = sqrt(d);
    
    double t1 = (-b + sqrtd) / (2. * a);
    double t2 = (-b - sqrtd) / (2. * a);
    
    
    // if both negative, return
    if (t1 <= RAY_OFFSET && t2 <= RAY_OFFSET) {
      ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;
      return false;
    }
      //if both positive choose closer
    else if (t1 > RAY_OFFSET && t2 > RAY_OFFSET) {
      if (t1 < t2) {
        tfar = t1;
      } else {
        tfar = t2;
      }
      //if one positive and other negative, choose positive
    } else {
      if (t1 < RAY_OFFSET) {
        tfar = t2;
      } else if (t2 < RAY_OFFSET) {
        tfar = t1;
      }
    }
  }
  
  if (ray.ray.tfar < RAY_OFFSET) {
    ray.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    return false;
  }
  ray.ray.tfar = tfar;
  ray.hit.geomID = RTC_COMPUTE_GEOMETRY_ID;
  
  return true;
  
  //  http://www-labs.iro.umontreal.ca/~sherknie/articles/faq_Divers/graphics-algorithms-faq.txt
  //  Given a ray defined as:
  //
  //      x = x1 + (x2 - x1)*t = x1 + i*t
  //      y = y1 + (y2 - y1)*t = y1 + j*t
  //      z = z1 + (z2 - z1)*t = z1 + k*t
  //
  //  and a sphere defined as:
  //
  //      (x - l)**2 + (y - m)**2 + (z - n)**2 = r**2
  //
  //  Substituting in gives the quadratic equation:
  //
  //      a*t**2 + b*t + c = 0
  //
  //  where:
  //
  //      a = i**2 + j**2 + k**2
  //      b = 2*i*(x1 - l) + 2*j*(y1 - m) + 2*k*(z1 - n)
  //      c = (x1-l)**2 + (y1-m)**2 + (z1-n)**2 - r**2;
  //
  //  If the discriminant of this equation is less than 0, the line does
  //  not intersect the sphere. If it is zero, the line is tangential to
  //  the sphere and if it is greater than zero it intersects at two
  //  points. Solving the equation and substituting the values of t into
  //  the ray equation will give you the points.
  
}

Sphere::Sphere(const glm::vec3 &worldPos, float radius, const std::shared_ptr<Material> &material) :
    worldPos_(worldPos),
    radius_(radius),
    material(material) {}

Sphere::Sphere() : Sphere(glm::vec3(0, 0, 0)) {}

glm::vec3 Sphere::getNormal(const RTCRayHitIor &ray) const {
  const glm::vec3 eyePos(ray.ray.org_x, ray.ray.org_y, ray.ray.org_z);
  const glm::vec3 rayDir(ray.ray.dir_x, ray.ray.dir_y, ray.ray.dir_z);
  const glm::vec3 impactPoint = eyePos + (rayDir * ray.ray.tfar);
  const glm::vec3 normal = glm::normalize(impactPoint - worldPos_);
  
  return normal;
}
