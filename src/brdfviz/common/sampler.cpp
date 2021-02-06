/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "sampler.h"
#include <common/utils/rng.h>

glm::vec3 Sampler::sample(const glm::vec3 &normal, float &pdf) const {
  const float randomU = rng();
  const float randomV = rng();
  return sampleImpl(randomU, randomV, normal, pdf);
}

glm::vec3 HemisphereSampler::sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const {
  const float x = 2.f * cosf(M_2PI * randomU) * sqrtf(randomV * (1.f - randomV));
  const float y = 2.f * sinf(M_2PI * randomU) * sqrtf(randomV * (1.f - randomV));
  const float z = 1.f - 2.f * randomV;
  
  glm::vec3 omegaI = glm::normalize(glm::vec3(x, y, z));
  
  if (glm::dot(omegaI, normal) < 0) {
//    omegaI *= -1;
    omegaI = -omegaI;
  }
  pdf = 1.f / M_2PI;
  return omegaI;
}
