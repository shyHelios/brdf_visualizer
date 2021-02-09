/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "sampler.h"
#include <common/utils/rng.h>
#include <common/utils/math.h>
#include <common/utils/coordsystem.h>


const std::pair<const char *, Sampler::Type> Sampler::samplerTypeArray[static_cast<int>(Type::Count)] = {
    {"Hemisphere",                         Sampler::Type::Hemisphere},
    {"Hemisphere (concentric cos weight)", Sampler::Type::HemisphereCosWeightConcentric},
    {"Specular hemisphere",                Sampler::Type::HemisphereGlossy},
    {"Phong",                              Sampler::Type::Phong},
};


Sampler::Sampler(const std::shared_ptr<BRDFShader> &brdfShaderPtr) : brdfShaderPtr(brdfShaderPtr) {}

glm::vec3 Sampler::sample(const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const {
  const float randomU = rng();
  const float randomV = rng();
  return sample(randomU, randomV, normal, reflectVector, pdf);
}


glm::vec3 Sampler::sample(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const {
  glm::vec3 omegaI;
  
  switch (currentType_) {
    case Type::Hemisphere: {
      omegaI = sampleImplHemisphere(randomU, randomV, normal, reflectVector, pdf);
      break;
    }
    case Type::HemisphereCosWeightConcentric: {
      omegaI = sampleImplHemisphereCosWeightConcentric(randomU, randomV, normal, reflectVector, pdf);
      break;
    }
    case Type::HemisphereGlossy: {
      omegaI = sampleImplHemisphereGlossy(randomU, randomV, normal, reflectVector, pdf);
      break;
    }
    case Type::Phong: {
      omegaI = sampleImplPhong(randomU, randomV, normal, reflectVector, pdf);
      break;
    }
  }
  
  return localToWorld(omegaI, normal);
}

Sampler::Type &Sampler::getCurrentType() {
  return currentType_;
}

void Sampler::setCurrentType(Sampler::Type currentType) {
  currentType_ = currentType;
}

glm::vec3 Sampler::sampleImplHemisphere(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const {
  const float theta = acos(randomU);
  const float phi = M_2PI * randomV;
  
  glm::vec3 omegaI = glm::normalize(sphericalToCartesian(theta, phi));
  
  pdf = 1.f / M_2PI;
  return omegaI;
}

glm::vec3 Sampler::sampleImplHemisphereCosWeightConcentric(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector,
                                                           float &pdf) const {
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

glm::vec3
Sampler::sampleImplHemisphereGlossy(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const {
  float n = brdfShaderPtr->getBrdfUniformLocations().shininess.getData();
  float theta = acos(std::pow(randomU, 1.f / (n + 1)));
  float phi = M_2PI * randomV;
  pdf = ((n + 1.f) / M_2PI) * std::pow(cos(theta), n);
  
  return localToWorld(sphericalToCartesian(theta, phi), reflectVector);
}

glm::vec3 Sampler::sampleImplPhong(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const {
  const float diffuse = brdfShaderPtr->getBrdfUniformLocations().diffuse.getData();
  const float specular = brdfShaderPtr->getBrdfUniformLocations().specular.getData();
  const float shininess = brdfShaderPtr->getBrdfUniformLocations().shininess.getData();
  const float randSample = rng();

//  const float n = brdfShaderPtr->getBrdfUniformLocations().shininess.getData();
  
  const float diffTheta = acos(randomU);
  const float specTheta = acos(std::pow(randomU, 1.f / (shininess + 1)));
  
  const float phi = M_2PI * randomV;
  
  const float diffusePdf = sqrt(randomU) / M_2PI;
  const float specularPdf = ((shininess + 1.f) / M_2PI) * std::pow(cos(specTheta), shininess);

//  const float pd = diffuse / (diffuse + specular);
//  const float ps = specular / (diffuse + specular);

//  pdf = pd * diffusePdf + ps * specularPdf;
  
  if (randSample < diffuse) {
    // take a diffuse sample and compute its contribution
    pdf = diffusePdf;
    return sphericalToCartesian(diffTheta, phi);
  } else {
    // take a specular sample and compute its contribution
    pdf = specularPdf;
    return localToWorld(sphericalToCartesian(specTheta, phi), reflectVector);
  }
}

bool Sampler::imguiSelectionGetter(void *data, int idx, const char **out_str) {
  *out_str = reinterpret_cast<std::pair<const char *, Type> *>(data)[idx].first;
  return true;
}


