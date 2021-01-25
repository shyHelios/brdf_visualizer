/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "rtlight.h"

RTLight::RTLight(const glm::vec3 &position, const glm::vec3 &color) : position_(position), color_(color) {}

const glm::vec3 &RTLight::getPosition() const {
  return position_;
}

const glm::vec3 &RTLight::getColor() const {
  return color_;
}
