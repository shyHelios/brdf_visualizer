/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/math.h>
#include <gl/texture.h>
#include "rtsphericalmap.h"

RTSphericalMap::RTSphericalMap(const std::string &filename) {
  texture_ = std::make_unique<Texture3f>(filename.c_str());
}

glm::vec3 RTSphericalMap::texel(const glm::vec3 &pos) {
  float theta = acosf(clamp(pos.z, -1.f, 1.f));
  float phi = atan2f(pos.y, pos.x) + static_cast<float>(M_PI);
  
  float u = 1.0f - phi * 0.5f * static_cast<float>(M_1_PI);
  float v = theta * static_cast<float>(M_1_PI);
  
  return colorToGlm(texture_->texel(u, v));
}