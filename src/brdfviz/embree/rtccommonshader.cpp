/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "rtccommonshader.h"
#include "mathscene.h"
#include "sphere.h"
#include "rtcamera.h"
#include "rtlight.h"
#include "gl/material.h"
#include "common/utils/math.h"
#include "common/utils/rng.h"


glm::vec4 RTCCommonShader::traceRay(const RTCRayHitIor &rayHit, int depth) {
  if (rayHit.hit.geomID == RTC_INVALID_GEOMETRY_ID) {
    return getBackgroundColor(rayHit);
  }
  glm::vec3 resultColor(0, 0, 0);
  if (depth <= 0) {
    //return from recursion, alpha 0 because division by alpha
    return glm::vec4(resultColor, 0.0f);
  }
  
  
  glm::vec3 normal;
  // and texture coordinates
  glm::vec2 tex_coord;
  //Acquire material from hit object
  std::shared_ptr<Material> material = nullptr;
  
  if (rayHit.hit.geomID == RTC_COMPUTE_GEOMETRY_ID) {
    resultColor = glm::vec3(1.f, 0.f, 0.5f);
    
    normal = mathScene_->getNormal(rayHit);
    material = mathScene_->getMaterial(rayHit);
    //return glm::vec4(normal, 1);
  } else if (rayHit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
    RTCGeometry geometry = rtcGetGeometry(rtcScene_, rayHit.hit.geomID);
    // get interpolated normal
    normal = glm::normalize(getNormal(geometry, rayHit));
    // and texture coordinates
    tex_coord = getTexCoords(geometry, rayHit);
    //Acquire material from hit object
    material = *static_cast<std::shared_ptr<Material> *>(rtcGetGeometryUserData(geometry));
  } else {
    spdlog::error("[RTC COMMON SHADER] Invalid geometry hit");
    return glm::vec4(0, 1, 1, 1);
  }
  
  /*
   * Common for all shaders
   * */
  const glm::vec3 origin(rayHit.ray.org_x, rayHit.ray.org_y, rayHit.ray.org_z);
//  const glm::vec3 direction(rayHit.ray.dir_x, rayHit.ray.dir_y, rayHit.ray.dir_z);
  const glm::vec3 direction = glm::normalize(glm::vec3(rayHit.ray.dir_x, rayHit.ray.dir_y, rayHit.ray.dir_z));
  const glm::vec3 worldPos = origin + direction * rayHit.ray.tfar;
  const glm::vec3 directionToCamera = -direction;
  const glm::vec3 lightPos = light_->getPosition();
  const glm::vec3 lightDir = glm::normalize(light_->getPosition() - worldPos);
  
  glm::vec3 shaderNormal = normal;
  
  float dotNormalCamera = glm::dot(shaderNormal, directionToCamera);
  //Flip normal if invalid
//  if (correctNormals_) {
//  if (dotNormalCamera < 0) {
//    shaderNormal *= -1.f;
//      dotNormalCamera *= -1.f;
//    dotNormalCamera = glm::dot(shaderNormal, directionToCamera);
//  }
//  assert(dotNormalCamera >= 0);
//  }
  
  /*
   * End of common for all shaders
   * */


//  const RTCShadingType selectedShader = (useShader == RTCShadingType::None) ? material->shadingType : useShader;
  const RTCShadingType selectedShader = useShader;
  
  switch (selectedShader) {
    case RTCShadingType::Glass: {
      resultColor = traceMaterial<RTCShadingType::Glass>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                         directionToCamera, lightPos, lightDir, shaderNormal,
                                                         dotNormalCamera, depth);
      break;
    }
    case RTCShadingType::Mirror: {
      resultColor = traceMaterial<RTCShadingType::Mirror>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                          directionToCamera, lightPos, lightDir, shaderNormal,
                                                          dotNormalCamera, depth);
      break;
    }
    case RTCShadingType::PathTracing: {
      resultColor = traceMaterial<RTCShadingType::PathTracing>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                               directionToCamera, lightPos, lightDir, shaderNormal,
                                                               dotNormalCamera, depth);
      break;
    }
    case RTCShadingType::Normals: {
      resultColor = traceMaterial<RTCShadingType::Normals>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                           directionToCamera, lightPos, lightDir, shaderNormal,
                                                           dotNormalCamera, depth);
      break;
    };
    case RTCShadingType::TexCoords: {
      resultColor = traceMaterial<RTCShadingType::TexCoords>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                             directionToCamera, lightPos, lightDir, shaderNormal,
                                                             dotNormalCamera, depth);
      break;
    };
    default:
    case RTCShadingType::None: {
      resultColor = traceMaterial<RTCShadingType::None>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                        directionToCamera, lightPos, lightDir, shaderNormal,
                                                        dotNormalCamera, depth);
      break;
    };
  }
  
  return glm::vec4(resultColor, 1.0f);
}

template<RTCShadingType T>
glm::vec4 RTCCommonShader::traceMaterial(const RTCRayHitIor &rayHit,
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
                                         const int depth) {
  
  spdlog::error("[RTC COMMON SHADER] Warning, no material");
  return glm::vec4(1, 0, 1, 1);
}

float
RTCCommonShader::getPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal, const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using Phong = BRDFShader::PhongUniformLocationsPack;
  glm::vec3 reflectVector = reflect(-toLight, normal);
  float specVal = std::pow(std::max(dot(toCamera, reflectVector), 0.0f), brdfShaderPtr->getBrdfUniformLocations().Phong::shininess.getData());
  return specVal;
}

float RTCCommonShader::getBlinnPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal,
                                         const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using Phong = BRDFShader::PhongUniformLocationsPack;
  glm::vec3 halfVector = normalize(toLight + toCamera);
  float specVal = std::pow(std::max(glm::dot(normal, halfVector), 0.0f), brdfShaderPtr->getBrdfUniformLocations().Phong::shininess.getData());
  return specVal;
}


float RTCCommonShader::beckmannDistribution(float roughness, float normDotHalf) {
  float roughness2 = roughness * roughness;
  float normDotHalf2 = normDotHalf * normDotHalf;
  float normDotHalf4 = normDotHalf2 * normDotHalf2;
  return std::exp((normDotHalf2 - 1) / (roughness2 * normDotHalf2)) / (roughness2 * normDotHalf2);
}

// https://en.wikipedia.org/wiki/Schlick%27s_approximation
float RTCCommonShader::schlick(float r0, float cosTheta) {
  return r0 + (1.f - r0) * std::pow(1.f - cosTheta, 5.f);
}

float RTCCommonShader::geometricAttenuation(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal) {
  glm::vec3 halfVector = normalize(toLight + toCamera);
  float normDotHalf = dot(normal, halfVector);
  float toCamDotHalf = dot(toCamera, halfVector);
  float normDotToCamera = dot(normal, toCamera);
  float normDotToLight = dot(normal, toLight);
  
  float res1 = (2.f * normDotHalf * normDotToCamera) / toCamDotHalf;
  float res2 = (2.f * normDotHalf * normDotToLight) / toCamDotHalf;
  
  float res = std::min(1.f, std::min(res1, res2));
  return res;
}

float RTCCommonShader::getTorranceSparrowBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal,
                                              const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using TorranceSparrow = BRDFShader::TorranceSparrowUniformLocationsPack;
  glm::vec3 halfVector = normalize(toLight + toCamera);
  
  float normDotHalf = dot(normal, halfVector);
  float toCamDotHalf = dot(toCamera, halfVector);
  
  float D = beckmannDistribution(brdfShaderPtr->getBrdfUniformLocations().TorranceSparrow::roughness.getData(), normDotHalf);
  float F = schlick(brdfShaderPtr->getBrdfUniformLocations().TorranceSparrow::f0.getData(), toCamDotHalf);
  float G = geometricAttenuation(toLight, toCamera, normal);
  
  float specVal = D * F * G;
  return specVal;
}


float RTCCommonShader::getBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal) {
  if (auto brdfShaderPtr = brdfShader.lock()) {
    switch (brdfShaderPtr->currentBrdfIdx) {
      case BRDFShader::BRDF::Phong:return getPhongBRDF(toLight, toCamera, normal, brdfShaderPtr);
      case BRDFShader::BRDF::BlinnPhong:return getBlinnPhongBRDF(toLight, toCamera, normal, brdfShaderPtr);
      case BRDFShader::BRDF::PhongPhysCorrect:return getPhysicallyCorrectPhongBRDF(toLight, toCamera, normal, brdfShaderPtr);
      case BRDFShader::BRDF::Lambert:return getLambertBRDF(toLight, toCamera, normal, brdfShaderPtr);
      case BRDFShader::BRDF::TorranceSparrow:return getTorranceSparrowBRDF(toLight, toCamera, normal, brdfShaderPtr);
      case BRDFShader::BRDF::OrenNayar:return getOrenNayarBRDF(toLight, toCamera, normal, brdfShaderPtr);
      default: {
        spdlog::warn("[COMMON SHADER] invalid BRDF selected");
        return 0;
      }
    }
  } else {
    spdlog::warn("[COMMON SHADER] cannot cock brdf shader");
    return 0;
  }
}

float RTCCommonShader::getLambertBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal,
                                      const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using Lambert = BRDFShader::LambertUniformLocationsPack;
  
  return brdfShaderPtr->getBrdfUniformLocations().Lambert::reflectance.getData() / M_PI;
}

float RTCCommonShader::getOrenNayarBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal,
                                        const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using OrenNayar = BRDFShader::OrenNayarUniformLocationsPack;
  
  float toCamDotNormal = dot(toCamera, normal);
  float toLightDotNormal = dot(toLight, normal);
  float rough2 =
      brdfShaderPtr->getBrdfUniformLocations().OrenNayar::roughness.getData() * brdfShaderPtr->getBrdfUniformLocations().OrenNayar::roughness.getData();
  
  float cosPhiri = dot(normalize(toCamera - normal * toCamDotNormal), normalize(toLight - normal * toLightDotNormal));
  
  float thetaI = acos(toLightDotNormal);
  float thetaO = acos(toCamDotNormal);
  
  float alpha = std::max(thetaI, thetaO);
  float beta = std::min(thetaI, thetaO);
  
  float C1 = 1. - 0.5 * (rough2 / (rough2 + 0.33));
  
  float C2;
  if (cosPhiri >= 0) {
    C2 = 0.45 * (rough2 / (rough2 + 0.09)) * sin(alpha);
  } else {
    C2 = 0.45 * (rough2 / (rough2 + 0.09)) * (sin(alpha) - pow(((2 * beta) / M_PI), 3));
  }
  
  float C3 = 0.125 * (rough2 / (rough2 + 0.09)) * ((4 * alpha * beta) / M_PI2);
  
  float L1r = (brdfShaderPtr->getBrdfUniformLocations().OrenNayar::reflectance.getData() / M_PI) *
              (C1 + cosPhiri * C2 * tan(beta) + (1. - abs(cosPhiri)) * C3 * tan((alpha + beta) / 2.));
  float L2r = 0.17 * ((brdfShaderPtr->getBrdfUniformLocations().OrenNayar::reflectance.getData() *
                       brdfShaderPtr->getBrdfUniformLocations().OrenNayar::reflectance.getData()) / M_PI) * (rough2 / (rough2 + 0.09)) *
              pow(((4 * alpha * beta) / M_PI2), 2);
  return L1r + L2r;
}

float RTCCommonShader::getPhysicallyCorrectPhongBRDF(const glm::vec3 &toLight, const glm::vec3 &toCamera, const glm::vec3 &normal,
                                                     const std::shared_ptr<BRDFShader> &brdfShaderPtr) {
  using Phong = BRDFShader::PhongUniformLocationsPack;
  glm::vec3 reflectVector = glm::reflect(-toLight, normal);
  float specVal = std::pow(std::max(glm::dot(toCamera, reflectVector), 0.0f), brdfShaderPtr->getBrdfUniformLocations().Phong::shininess.getData());
  //  return (u_phongDiffuse / M_PI) + (((u_phongSpecular * (u_phongShininess + 2)) / M_2PI) * pow(max(dot(toCamera, reflectVector), 0.0), u_phongShininess));
  return (brdfShaderPtr->getBrdfUniformLocations().Phong::diffuse.getData() / M_PI) +
         (((brdfShaderPtr->getBrdfUniformLocations().Phong::specular.getData() * (brdfShaderPtr->getBrdfUniformLocations().Phong::shininess.getData() + 2)) /
           M_2PI) * specVal);
  
}


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
                                                               const int depth) {
  
  return glm::vec4(colorToGlm(material->diffuse_), 1);
//  return glm::vec4(1.f, 0.f, 0.f, 1.f);
}

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
                                                                const int depth) {
  
  
  float materialIor;
//  if (useShader == RTCShadingType::Glass) {
//    materialIor = ior;
//  } else {
  assert(material->ior >= 0);
  materialIor = material->ior;
//  }
  
  if (depth <= 0) {
    return getBackgroundColor(rayHit);
  }
  
  glm::vec4 reflected(0.f, 0.f, 0.f, 1.f);
  glm::vec4 refracted(0.f, 0.f, 0.f, 1.f);
  
  //n1 = ray ior
  //n2 = material ior
  //if n1 != vzduch than n2 = air
  float n1 = rayHit.ior;
  float n2 = materialIor;
  
  if (n1 != IOR_AIR) {
    n2 = IOR_AIR;
  }
  
  assert(n1 >= 0);
  assert(n2 >= 0);
  //0.64 1.54
  const float n1overn2 = (n1 / n2);
//cos1
  float Q1 = dotNormalCamera;
  
  assert(Q1 >= 0);
  
  const glm::vec3 reflectDir = glm::normalize(
      (2.f * (directionToCamera * shaderNormal)) * shaderNormal - directionToCamera);
  
  RTCRayHitIor reflectedRayHit = generateRay(worldPos, reflectDir, tNear_);
  reflectedRayHit.ior = n2;
  
  const float tmp = 1.f - sqr(n1overn2) * (1.f - sqr(Q1));
  if (tmp < 0.f) {
    return traceRay(reflectedRayHit, depth - 1);
  }

//cos2
  const float Q2 = sqrtf(tmp);
  const glm::vec3 refractedDir = glm::normalize(
      (n1overn2 * direction) + ((n1overn2 * Q1 - Q2) * shaderNormal));
// Fresnel
  const float R = RTCShader::fresnel(n1, n2, Q1, Q2);
  const float coefReflect = R;
  const float coefRefract = 1.f - R;
  
  RTCRayHitIor refractedRayHit = generateRay(worldPos, refractedDir, tNear_);
  refractedRayHit.ior = n2;
  
  reflected = traceRay(reflectedRayHit, depth - 1);
  refracted = traceRay(refractedRayHit, depth - 1);
  
  assert(coefReflect >= 0.f);
  assert(coefReflect <= 1.f);
  
  assert(coefRefract >= 0.f);
  assert(coefRefract <= 1.f);
  
  //C = [Crefl*R + Crefr*(1-R)] * TbeerLambert
  //TbeerLambert = {1,1,1} for air
  //TbeerLambert = {e^-Mr*l,e^-Mg*l,e^-Mb*l} for air, l = delka paprsku, M = absorpce materialu
  glm::vec3 TBeerLambert;
  if (rayHit.ior == IOR_AIR) {
    TBeerLambert = {1.f, 1.f, 1.f};
  } else {
    const float l = rayHit.ray.tfar;
    const glm::vec3 M = {10, 0.5, 10};
//    const glm::vec3 M = material->absorption;
    TBeerLambert = {
        expf(-M.r * l),
        expf(-M.g * l),
        expf(-M.b * l)
    };
  }
  
  const glm::vec3 finalReflect = coefReflect * glm::vec3(reflected.x, reflected.y, reflected.z);
  const glm::vec3 finalRefract = coefRefract * glm::vec3(refracted.x, refracted.y, refracted.z);
  
  return glm::vec4((finalReflect + finalRefract) * TBeerLambert, 1);

//  return coefReflect * reflected + coefRefract * refracted * glm::vec4(TBeerLambert, 1.f);
}

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
                                                                 const int depth) {
  if (rayHit.hit.geomID == RTC_COMPUTE_GEOMETRY_ID) {
    (void) 0; // my breakpint
  }
  //I - N * dot(N, I) * 2
  glm::vec3 reflectDir = glm::reflect(direction, shaderNormal);
  
  glm::vec4 reflected(0.f, 0.f, 0.f, 1.f);
  
  RTCRayHitIor reflectedRayHit = generateRay(worldPos, reflectDir, tNear_);
  reflected = traceRay(reflectedRayHit, depth - 1);
  return reflected;
}

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
                                                                      const int depth) {
//  return glm::vec4(material->diffuse_.data[0], material->diffuse_.data[1], material->diffuse_.data[2], 1);
  
  const int currentRecursion = recursionDepth_ - depth;
  // Russian roulette
  float rho = (material == nullptr) ? 0.95 : (
      (std::max<float>(material->diffuse_.data[0], std::max<float>(material->diffuse_.data[1], material->diffuse_.data[2]))) * 0.95f);
  
  
  if (rho <= rng()) {
    return glm::vec4(0, 0, 0, 0);
  }
  
  if (brdfShader.lock()->currentBrdfIdx == BRDFShader::BRDF::Mirror) {
    return traceMaterial<RTCShadingType::Mirror>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                 directionToCamera, lightPos, lightDir, shaderNormal,
                                                 dotNormalCamera, depth);
  }
  
  const glm::vec3 emmision =
      (material == nullptr) ? glm::vec3(0, 0, 0) : glm::vec3{material->emission_.data[0], material->emission_.data[1], material->emission_.data[2]};
  
  float pdf = 1;
  const glm::vec3 reflectDir = glm::reflect(direction, shaderNormal);
  const glm::vec3 omegaI = pathTracerHelper->getTracesCount() == 0 ? reflectDir : sampler_->sample(shaderNormal, reflectDir, pdf);
  
  if (pdf <= 0) {
    return glm::vec4(0, 0, 0, 0);
  }
  
  const float brdf = getBRDF(omegaI, directionToCamera, shaderNormal);
  const RTCRayHitIor rayHitNew = generateRay(worldPos, omegaI);
  
  const glm::vec4 li = traceRay(rayHitNew, depth - 1);
  const glm::vec3 diffuse = getDiffuseColor(material, tex_coord);
  const glm::vec4 diffuse4 = glm::vec4(diffuse.x, diffuse.y, diffuse.z, 1);
  glm::vec3 finalColor = diffuse4 * li * brdf * glm::dot(shaderNormal, omegaI) / (pdf * rho);
  return glm::vec4(finalColor.x, finalColor.y, finalColor.z, 1);
}

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
                                                                  const int depth) {
  
  //Debug dot normal
//  return glm::vec4(glm::vec3(dotNormalCamera), 1.0f);
  return glm::vec4((shaderNormal.x + 1.f) / 2.f,
                   (shaderNormal.y + 1.f) / 2.f,
                   (shaderNormal.z + 1.f) / 2.f,
                   1.0f);
}

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
                                                                    const int depth) {
  
  
  return glm::vec4(tex_coord.x, tex_coord.y, 1.0, 1.0f);
}