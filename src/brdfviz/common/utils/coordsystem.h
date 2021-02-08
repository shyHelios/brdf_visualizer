/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef COORDSYSTEM_H
#define COORDSYSTEM_H

inline glm::vec3 sphericalToCartesian(const float theta, const float phi) {
  return glm::vec3(sin(theta) * cos(phi),
                   sin(theta) * sin(phi),
                   cos(theta));
}

inline glm::vec3 sphericalToCartesian(const glm::vec2 vec) {
  return sphericalToCartesian(vec.x, vec.y);
}


inline glm::vec2 cartesianToSpherical(const float x, const float y, const float z) {
  float theta = atan((sqrt((x * x) + (y * y))) / z);
  float phi = atan(y / x);
  if (std::isnan(theta)) {
    theta = 0;
  }
  if (std::isnan(phi)) {
    phi = 0;
  }
  return glm::vec2(theta, phi);
}

inline glm::vec2 cartesianToSpherical(const glm::vec3 &vec) {
  return cartesianToSpherical(vec.x, vec.y, vec.z);
}

void coordSystem(const glm::vec3 &normal, glm::vec3 &tangent, glm::vec3 &bitangent);

glm::mat3 coordTransformMatrix(const glm::vec3 &normal);

glm::vec3 localToWorld(const glm::vec3 &sample, const glm::vec3 &normal);

#endif //COORDSYSTEM_H
