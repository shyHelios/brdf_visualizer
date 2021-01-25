/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCSHADER_H
#define RTCSHADER_H


//#include <vector>
//#include <array>
////#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include <glm/vec4.hpp>

#include "rtcrayhitior.h"
#include "rtcamera.h"
#include "rtlight.h"
#include "rtsphericalmap.h"
#include "pathtracerhelper.h"
#include "mathscene.h"
#include "gl/material.h"

//class RTSurface;

enum class SuperSamplingType : int {
  None = 0,
  Uniform,
  RandomFinite,
  RandomInfinite,
  SuperSamplingCount
};


class RTCShader {
public:
  RTCShader();
  
  std::unique_ptr<PathTracerHelper> pathTracerHelper;
  bool flipTextureU_;
  bool flipTextureV_;
  bool sphereMap_;
  int samplingSize_;
  int samplingSizeX_;
  int samplingSizeY_;
  int recursionDepth_;
  float tNear_;
  SuperSamplingType superSamplingType_;
  std::unique_ptr<RTCamera> camera_;
  std::unique_ptr<MathScene> mathScene_;
  
  
  virtual glm::vec4 getPixel(int x, int y);
  
  RTCRayHitIor shootRay(float x, float y);
  
  glm::vec3 getDiffuseColor(const std::shared_ptr<Material> &material, const glm::vec2 &tex_coord);
  
  static float fresnel(float n1, float n2, float Q1, float Q2);

protected:
  std::unique_ptr<RTLight> light_;
  std::unique_ptr<RTSphericalMap> sphericalMap_;
  glm::vec4 defaultBgColor_;

//  static glm::vec3 getNormal(const std::unique_ptr<RTCGeometryTy> &geometry, const RTCRayHitIor &rayHit);
//
//  static glm::vec2 getTexCoords(const std::unique_ptr<RTCGeometryTy> &geometry, const RTCRayHitIor &rayHit);
  
  float shadow(const glm::vec3 &pos, const glm::vec3 &lightDir, const float dist);
  
  RTCRayHitIor generateRay(const glm::vec3 &origin, const glm::vec3 &direction, float tnear = 0.001f);
  
  virtual glm::vec4 traceRay(const RTCRayHitIor &rayHit, int depth);
  
  glm::vec4 getBackgroundColor(const RTCRayHitIor &rayHit);
  
  glm::vec3 hemisphereSampling(const glm::vec3 &normal, float &pdf);
};


#endif //RTCSHADER_H
