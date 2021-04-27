/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCCOMMONSHADER_H
#define RTCCOMMONSHADER_H

#include <gl/shaders/brdfshader.h>
#include "rtcshader.h"
#include "rtcshadingtype.h"

class RTCCommonShader : public RTCShader {
public:
  glm::vec4 traceRay(const RTCRayHitIor &rayHit, int depth) override;
  
  RTCShadingType useShader;
  
  float getBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal);
  
  float getPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getBlinnPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float
  getPhysicallyCorrectPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getLambertBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getTorranceSparrowBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getCookTorranceBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getOrenNayarBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  float getMirrorBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr);
  
  glm::vec4 sampleLight(const glm::vec3 &direction,
                        const glm::vec3 &shaderNormal,
                        const glm::vec3 &directionToCamera,
                        const glm::vec3 &worldPos,
                        const int depth,
                        float &pdf);
  
  glm::vec4 sampleBRDF(const glm::vec3 &direction,
                       const glm::vec3 &shaderNormal,
                       const glm::vec3 &directionToCamera,
                       const glm::vec3 &worldPos,
                       const int depth,
                       float &pdf);
  
  std::weak_ptr<BRDFShader> brdfShader;
protected:
  float beckmannDistribution(float roughness, float normDotHalf);
  
  float schlick(float r0, float cosTheta);
  
  float geometricAttenuation(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal);

private:
  
  template<RTCShadingType T>
  glm::vec4 traceMaterial(const RTCRayHitIor &rayHit,
                          const std::shared_ptr<Material> &material,
                          const glm::vec2 &tex_coord,
                          const glm::vec3 &origin,
                          const glm::vec3 &direction,
                          const glm::vec3 &worldPos,
                          const glm::vec3 &directionToCamera,
                          const glm::vec3 &lightPos,
                          const glm::vec3 &lightDir,
                          const glm::vec3 &shaderNormal,
                          const float dotNormalCamera,
                          const int depth);
};

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::None>(const RTCRayHitIor &rayHit,
                                                               const std::shared_ptr<Material> &material,
                                                               const glm::vec2 &tex_coord,
                                                               const glm::vec3 &origin,
                                                               const glm::vec3 &direction,
                                                               const glm::vec3 &worldPos,
                                                               const glm::vec3 &directionToCamera,
                                                               const glm::vec3 &lightPos,
                                                               const glm::vec3 &lightDir,
                                                               const glm::vec3 &shaderNormal,
                                                               const float dotNormalCamera,
                                                               const int depth);

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::Glass>(const RTCRayHitIor &rayHit,
                                                                const std::shared_ptr<Material> &material,
                                                                const glm::vec2 &tex_coord,
                                                                const glm::vec3 &origin,
                                                                const glm::vec3 &direction,
                                                                const glm::vec3 &worldPos,
                                                                const glm::vec3 &directionToCamera,
                                                                const glm::vec3 &lightPos,
                                                                const glm::vec3 &lightDir,
                                                                const glm::vec3 &shaderNormal,
                                                                const float dotNormalCamera,
                                                                const int depth);

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::Mirror>(const RTCRayHitIor &rayHit,
                                                                 const std::shared_ptr<Material> &material,
                                                                 const glm::vec2 &tex_coord,
                                                                 const glm::vec3 &origin,
                                                                 const glm::vec3 &direction,
                                                                 const glm::vec3 &worldPos,
                                                                 const glm::vec3 &directionToCamera,
                                                                 const glm::vec3 &lightPos,
                                                                 const glm::vec3 &lightDir,
                                                                 const glm::vec3 &shaderNormal,
                                                                 const float dotNormalCamera,
                                                                 const int depth);

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::PathTracing>(const RTCRayHitIor &rayHit,
                                                                      const std::shared_ptr<Material> &material,
                                                                      const glm::vec2 &tex_coord,
                                                                      const glm::vec3 &origin,
                                                                      const glm::vec3 &direction,
                                                                      const glm::vec3 &worldPos,
                                                                      const glm::vec3 &directionToCamera,
                                                                      const glm::vec3 &lightPos,
                                                                      const glm::vec3 &lightDir,
                                                                      const glm::vec3 &shaderNormal,
                                                                      const float dotNormalCamera,
                                                                      const int depth);

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::Normals>(const RTCRayHitIor &rayHit,
                                                                  const std::shared_ptr<Material> &material,
                                                                  const glm::vec2 &tex_coord,
                                                                  const glm::vec3 &origin,
                                                                  const glm::vec3 &direction,
                                                                  const glm::vec3 &worldPos,
                                                                  const glm::vec3 &directionToCamera,
                                                                  const glm::vec3 &lightPos,
                                                                  const glm::vec3 &lightDir,
                                                                  const glm::vec3 &shaderNormal,
                                                                  const float dotNormalCamera,
                                                                  const int depth);

template<>
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::TexCoords>(const RTCRayHitIor &rayHit,
                                                                    const std::shared_ptr<Material> &material,
                                                                    const glm::vec2 &tex_coord,
                                                                    const glm::vec3 &origin,
                                                                    const glm::vec3 &direction,
                                                                    const glm::vec3 &worldPos,
                                                                    const glm::vec3 &directionToCamera,
                                                                    const glm::vec3 &lightPos,
                                                                    const glm::vec3 &lightDir,
                                                                    const glm::vec3 &shaderNormal,
                                                                    const float dotNormalCamera,
                                                                    const int depth);


#endif //RTCCOMMONSHADER_H
