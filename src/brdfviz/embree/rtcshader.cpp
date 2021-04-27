/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "rtcshader.h"
#include <common/utils/rng.h>
#include <common/utils/math.h>
#include <gl/material.h>

#include "rtcamera.h"
#include "rtlight.h"
#include "rtsphericalmap.h"
#include "sphere.h"
#include "pathtracerhelper.h"
#include "mathscene.h"

const std::pair<const char *, Sampling> RTCShader::samplingArray[static_cast<int>(Sampling::SamplingCount)] = {
    {"BRDF",   Sampling::BRDF},
    {"Lights", Sampling::Lights},
    {"MIS",    Sampling::MIS},
};


RTCShader::RTCShader() :
    pathTracerHelper(nullptr),
    flipTextureU_(false),
    flipTextureV_(false),
    sphereMap_(true),
    samplingSize_(3),
    samplingSizeX_(3),
    samplingSizeY_(3),
    recursionDepth_(3),
    tNear_(0.001f),
    superSamplingType_(SuperSamplingType::RandomInfinite),
    camera_(nullptr),
    mathScene_(nullptr),
    light_(std::make_unique<RTLight>(glm::vec3(10, 10, 10))),
//    sphericalMap_(std::make_unique<RTSphericalMap>("data/sphereMap.jpg")),
//    sphericalMap_(std::make_unique<RTSphericalMap>("data/studio_small_03_4k.hdr")),
    sphericalMap_(std::make_unique<RTSphericalMap>("data/outdoor_umbrellas_4k.hdr")),
    defaultBgColor_(glm::vec4(0.8, 0.8, 0.8, 1)),
    samplingType_(Sampling::MIS) {
}

RTCRayHitIor RTCShader::shootRay(const float x, const float y) {
  RTCRayHitIor rayHit = generateRay(
      camera_->getViewFrom(),
      camera_->getRayDirection(x + 0, y + 0),
      FLT_MIN);
  return rayHit;
}

glm::vec3 RTCShader::getDiffuseColor(const std::shared_ptr<Material> &material, const glm::vec2 &tex_coord) {
  const auto &diffTexture = material->texture(TextureSlot::diffuseSlot);
  glm::vec3 geomDiffuse;
  
  if (diffTexture != nullptr) {
    glm::vec2 correctedTexCoord = tex_coord;
    
    if (flipTextureU_)
      correctedTexCoord.x = 1.f - correctedTexCoord.x;
    if (flipTextureV_)
      correctedTexCoord.y = 1.f - correctedTexCoord.y;
    
    geomDiffuse = colorToGlm(diffTexture->texel(correctedTexCoord.x, correctedTexCoord.y));
  } else {
    geomDiffuse = colorToGlm(material->diffuse_);
  }
  return geomDiffuse;
}

glm::vec4 RTCShader::getPixel(const int x, const int y) {
  glm::vec4 finalColor(0, 0, 0, 0);
  
  float offsetX;
  float offsetY;
  
  offsetX = rng(-0.5f, 0.5f);
  offsetY = rng(-0.5f, 0.5f);
  RTCRayHitIor ray = shootRay(static_cast<float>(x) + offsetX, static_cast<float>(y) + offsetY);
  pathTracerHelper->setPixel(y, x, traceRay(ray, recursionDepth_));
  
  finalColor = pathTracerHelper->getInterpolatedPixel(y, x);
  
  return finalColor / finalColor.a;
  
}

glm::vec4 RTCShader::traceRay(const RTCRayHitIor &rayHit, int depth) {
  if (rayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID) {
    return getBackgroundColor(rayHit);
  }
  
  return glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
}

glm::vec4 RTCShader::getBackgroundColor(const RTCRayHitIor &rayHit) {
  if (sphericalMap_ != nullptr && sphereMap_) {
    const glm::vec3 rayDir = glm::normalize(glm::vec3(rayHit.ray.dir_x, rayHit.ray.dir_y, rayHit.ray.dir_z));
    return glm::vec4(sphericalMap_->texel(rayDir), 1.0f);
  }
  
  return glm::vec4(defaultBgColor_.x, defaultBgColor_.y, defaultBgColor_.z, 1.0f);
}

RTCRayHitIor RTCShader::generateRay(const glm::vec3 &origin,
                                    const glm::vec3 &direction,
                                    const float tnear) {
  RTCRayHitIor rayHit;
  
  rayHit.ray.org_x = origin.x;     // x coordinate of ray origin
  rayHit.ray.org_y = origin.y;     // y coordinate of ray origin
  rayHit.ray.org_z = origin.z;     // z coordinate of ray origin
  rayHit.ray.tnear = tnear;        // start of ray segment
  rayHit.ray.dir_x = direction.x;  // x coordinate of ray direction
  rayHit.ray.dir_y = direction.y;  // y coordinate of ray direction
  rayHit.ray.dir_z = direction.z;  // z coordinate of ray direction
  rayHit.ray.time = 0;             // time of this ray for motion blur
  rayHit.ray.tfar = FLT_MAX;       // end of ray segment (set to hit distance)
  rayHit.ray.mask = 0;             // ray mask
  rayHit.ray.id = 0;               // ray ID
  rayHit.ray.flags = 0;            // ray flags
  rayHit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayHit.hit.primID = RTC_INVALID_GEOMETRY_ID;
  rayHit.hit.Ng_x = 0.0f; // geometry normal
  rayHit.hit.Ng_y = 0.0f;
  rayHit.hit.Ng_z = 0.0f;
  
  if (rtcScene_ != nullptr) {
    /*
     * Embree intersect
     * */
    // intersect ray with the scene
    
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    rtcIntersect1(rtcScene_, &context, &rayHit);
  }
  
  
  if (mathScene_ != nullptr) {
    /*
     * My intersect
     * */
    mathScene_->intersect(rayHit);
  }
  
  return rayHit;
}

glm::vec3 RTCShader::getNormal(const RTCGeometry geometry, const RTCRayHitIor &rayHit) {
  glm::vec3 normal;
  normal.x = rayHit.hit.Ng_x;
  normal.y = rayHit.hit.Ng_y;
  normal.z = rayHit.hit.Ng_z;
//  rtcInterpolate0(geometry, rayHit.hit.primID, rayHit.hit.u, rayHit.hit.v,
//                  RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, NORMAL_SLOT, &normal.x, 3);
  return normal;
  
}

glm::vec2 RTCShader::getTexCoords(const RTCGeometry geometry, const RTCRayHitIor &rayHit) {
  glm::vec2 tex_coord;
  rtcInterpolate0(geometry, rayHit.hit.primID, rayHit.hit.u, rayHit.hit.v,
                  RTC_BUFFER_TYPE_VERTEX_ATTRIBUTE, TEXCOORD_SLOT, &tex_coord.x, 2);
  return tex_coord;
}

float RTCShader::fresnel(const float n1, const float n2, const float Q1, const float Q2) {
  float Rs = sqr((n1 * Q2 - n2 * Q1) / (n1 * Q2 + n2 * Q1));
  float Rp = sqr((n1 * Q1 - n2 * Q2) / (n1 * Q1 + n2 * Q2));
  
  return 0.5f * (Rs + Rp);
}


float RTCShader::shadow(const glm::vec3 &pos, const glm::vec3 &lightDir, const float dist) {
  RTCHit hit;
  hit.geomID = RTC_INVALID_GEOMETRY_ID;
  hit.primID = RTC_INVALID_GEOMETRY_ID;
  
  RTCRay ray = RTCRay();
  ray.org_x = pos.x; // ray origin
  ray.org_y = pos.y;
  ray.org_z = pos.z;
  
  ray.dir_x = lightDir.x;
  ray.dir_y = lightDir.y;
  ray.dir_z = lightDir.z;
  
  ray.tnear = 0.1f;
  ray.tfar = dist;
  
  ray.time = 0.0f;
  
  ray.mask = 0; // can be used to mask out some geometries for some rays
  ray.id = 0; // identify a ray inside a callback function
  ray.flags = 0; // reserved
  
  if (rtcScene_ != nullptr) {
    /*
     * Embree intersect
     * */
    // intersect ray with the scene
    
    
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    rtcOccluded1(rtcScene_, &context, &ray);
  }
  
  if (mathScene_ != nullptr) {
    /*
     * My intersect
     * */
    mathScene_->occlude(ray);
  }


//  spdlog::warn("[EMBREE SHADER] Shadows removed, always false");
  
  if (ray.tfar < dist) {
    return 0.0;
  } else {
    return 1.0;
  }
}

bool &RTCShader::getUseSphereMapRef() {
  return sphereMap_;
}

glm::vec4 &RTCShader::getDefaultBgColorRef() {
  return defaultBgColor_;
}

const std::shared_ptr<Sampler> &RTCShader::getSampler() const {
  return sampler_;
}

void RTCShader::setSampler(const std::shared_ptr<Sampler> &sampler) {
  sampler_ = sampler;
}

Sampling &RTCShader::getSamplingTypeRef() {
  return samplingType_;
}

