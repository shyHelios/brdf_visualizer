/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SAMPLER_H
#define SAMPLER_H

#include <gl/shaders/brdfshader.h>

class Sampler {
public:
  enum class Type {
    Hemisphere,
    HemisphereCosWeightConcentric,
    HemisphereGlossy,
    Phong,
    Count
  };
  
  Sampler(const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  friend class SamplerVisualizerObject;
  
  glm::vec3 sample(const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
  
  glm::vec3 sample(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
  
  Type &getCurrentType();
  
  void setCurrentType(Type currentType);
  
  static const std::pair<const char *, Type> samplerTypeArray[static_cast<int>(Type::Count)];
  
  static bool imguiSelectionGetter(void *data, int idx, const char **out_str);

protected:
  Type currentType_ = Type::HemisphereCosWeightConcentric;
  const std::shared_ptr<BRDFShader> brdfShaderPtr;

private:
  glm::vec3 sampleImplHemisphere(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
  
  glm::vec3
  sampleImplHemisphereCosWeightConcentric(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
  
  glm::vec3 sampleImplHemisphereGlossy(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
  
  glm::vec3 sampleImplPhong(const float randomU, const float randomV, const glm::vec3 &normal, const glm::vec3 &reflectVector, float &pdf) const;
};

#endif //SAMPLER_H
