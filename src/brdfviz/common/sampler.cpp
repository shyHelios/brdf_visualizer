/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "sampler.h"
#include <common/utils/rng.h>
#include <common/utils/math.h>
#include <common/utils/coordsystem.h>

glm::vec3 Sampler::sample(const glm::vec3 &normal, float &pdf) const {
  const float randomU = rng();
  const float randomV = rng();
  return localToWorld(sampleImpl(randomU, randomV, normal, pdf), normal);
}

glm::vec3 HemisphereSampler::sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const {
  const float theta = acos(randomU);
  const float phi = M_2PI * randomV;
  
  glm::vec3 omegaI = glm::normalize(sphericalToCartesian(theta, phi));
  
  pdf = 1.f / M_2PI;
  return omegaI;
}

glm::vec3 PhongSampler::sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const {
  /*const */float diffuse;
  /*const */float shininess;
  const float randSample = rng();
  if (randSample < diffuse) {
    // take a diffuse sample and compute its contribution
  } else {
    // take a specular sample and compute its contribution
    float alpha = std::acos(pow(randomU, (1. / (shininess - 1.))));
    float phi = M_2PI * shininess;
    
    pdf = ((shininess + 1.f) / M_2PI) * (powf(std::cos(alpha), shininess));
  }
  
  
  return glm::vec3();
}

glm::vec3 HemisphereCosWeightedSampler::sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const {
  // Map from <0,1> to <-1,1>
  const float randomUMapped = (randomU * 2.f) - 1.f;
  const float randomVMapped = (randomV * 2.f) - 1.f;
  
  float concentricTheta;
  float concentricPhi;
  if (randomUMapped == 0 && randomVMapped == 0) {
    concentricTheta = 0;
    concentricPhi = 0;
  } else {
    //concentric mapping
    float concTh, r;
    if (std::abs(randomUMapped) > std::abs(randomVMapped)) {
      r = randomUMapped;
      concTh = (M_PI / 4.f) * (randomVMapped / randomUMapped);
    } else {
      r = randomVMapped;
      concTh = (M_PI / 2.f) - (M_PI / 4.f) * (randomUMapped / randomVMapped);
    }
    
    concentricTheta = r * std::cos(concTh);
    concentricPhi = r * std::sin(concTh);
  }
  
  
  float z = std::sqrt(std::max((float) 0, 1 - sqr(concentricTheta) - sqr(concentricPhi)));
  
  return glm::vec3(concentricTheta, concentricPhi, z);
}
