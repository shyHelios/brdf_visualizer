/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCCOMMONSHADER_H
#define RTCCOMMONSHADER_H

#include "rtcshader.h"
#include "rtcshadingtype.h"

class RTCCommonShader : public RTCShader {
public:
  glm::vec4 traceRay(const RTCRayHitIor &rayHit, int depth) override;
  
  RTCShadingType useShader;
protected:

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
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::Lambert>(const RTCRayHitIor &rayHit,
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
glm::vec4 RTCCommonShader::traceMaterial<RTCShadingType::Phong>(const RTCRayHitIor &rayHit,
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
