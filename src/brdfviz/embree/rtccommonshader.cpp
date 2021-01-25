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
  } else {
    spdlog::error("[RTC COMMON SHADER] Invalid geometry hit");
    return glm::vec4(0, 1, 1, 1);
  }
  /*else {
    RTCGeometry geometry = rtcGetGeometry(*rtcScene_, rayHit.hit.geomID);
    // get interpolated normal
    normal = glm::normalize(getNormal(geometry, rayHit));
    // and texture coordinates
    tex_coord = getTexCoords(geometry, rayHit);
    //Acquire material from hit object
    material = static_cast<Material *>(rtcGetGeometryUserData(geometry));
  }*/
  
  
  
  
  
  
  
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
    case RTCShadingType::None: {
      resultColor = traceMaterial<RTCShadingType::None>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                        directionToCamera, lightPos, lightDir, shaderNormal,
                                                        dotNormalCamera, depth);
      break;
    }
    
    case RTCShadingType::Glass: {
      resultColor = traceMaterial<RTCShadingType::Glass>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                         directionToCamera, lightPos, lightDir, shaderNormal,
                                                         dotNormalCamera, depth);
      break;
    }
    case RTCShadingType::Lambert: {
      resultColor = traceMaterial<RTCShadingType::Lambert>(rayHit, material, tex_coord, origin, direction, worldPos,
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
    case RTCShadingType::Phong: {
      resultColor = traceMaterial<RTCShadingType::Phong>(rayHit, material, tex_coord, origin, direction, worldPos,
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
  
  
  return glm::vec4(1.f, 0.f, 0.f, 1.f);
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
                                                                  const int depth) {
  glm::vec3 diffuse = dotNormalCamera * getDiffuseColor(material, tex_coord);
  
  //shadow
  float shadowVal = 1;
  float pdf = 0;

//  if (softShadows) {
//
//    shadowVal += shadow(worldPos, lightDir, glm::l2Norm(lightPos - worldPos));
//
//    for (int i = 0; i < lightShadowsSamples; i++) {
//      const glm::vec3 lDir = hemisphereSampling(lightDir, pdf);
//      shadowVal += shadow(worldPos, lDir, glm::l2Norm(lightPos - worldPos));
//    }
//
//    shadowVal /= static_cast<float>(lightShadowsSamples + 1);
//  } else {
  //hard shadow
  shadowVal = shadow(worldPos, lightDir, glm::l2Norm(lightPos));
  //}
  
  const float dotNormalLight = glm::dot(shaderNormal, lightDir);
  
  return glm::vec4(
      ((diffuse.x * shadowVal * dotNormalLight)),
      ((diffuse.y * shadowVal * dotNormalLight)),
      ((diffuse.z * shadowVal * dotNormalLight)),
      1);
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
                                                                const int depth) {
  
  
  glm::vec4 reflected = traceMaterial<RTCShadingType::Mirror>(rayHit, material, tex_coord, origin, direction, worldPos,
                                                              directionToCamera, lightPos, lightDir, shaderNormal,
                                                              dotNormalCamera, depth);
  
  if (material->reflectivity >= 1.0) {
    return reflected;
  }
  
  //ambient
  glm::vec3 ambient = colorToGlm(material->ambient_);
  
  //diffuse
  float shadowVal = 0;
  float pdf = 0;

//  if (softShadows) {
//
//    shadowVal += shadow(worldPos, lightDir, glm::l2Norm(lightPos - worldPos));
//
//    for (int i = 0; i < lightShadowsSamples; i++) {
//      const glm::vec3 lDir = hemisphereSampling(lightDir, pdf);
//      shadowVal += shadow(worldPos, lDir, glm::l2Norm(lightPos - worldPos));
//    }
//
//    shadowVal /= static_cast<float>(lightShadowsSamples + 1);
//  } else {
  //hard shadow
  shadowVal = shadow(worldPos, lightDir, glm::l2Norm(lightPos));
//  }
  
  glm::vec3 diffuse = shadowVal * dotNormalCamera * getDiffuseColor(material, tex_coord);
  
  //specular
  //I - N * dot(N, I) * 2
  //glm::vec3 lightReflectDir = glm::reflect(lightDir, shaderNormal);
//  float spec = powf(glm::dot(direction, lightReflectDir), material->shininess);
  
  //blinn-phong lightning
  glm::vec3 halfwayDir = glm::normalize(lightDir + direction);
  float spec = powf(std::max<float>(glm::dot(shaderNormal, halfwayDir), 0.0), material->shininess);
  
  glm::vec3 specular = colorToGlm(material->specular_) * spec;

//  glm::vec3 torranceSparrow = TorranceSparrowBRDF::getBRDF(material, shaderNormal, lightDir, directionToCamera,
//                                                           worldPos);

//  return glm::vec4(std::max<float>(glm::dot(shaderNormal, lightDir), 0) * torranceSparrow, 1);
  return glm::vec4(diffuse + specular * spec, 1) + (reflected * material->reflectivity/* * spec*/);
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
  const int currentRecursion = recursionDepth_ - depth;
  if (currentRecursion > 2) {
    if (rng() >
        (std::max<float>(material->diffuse_.data[0], std::max<float>(material->diffuse_.data[1], material->diffuse_.data[2]))) * 0.95f) {
      return glm::vec4(0, 0, 0, 0);
    }
  }
  
  glm::vec3 emmision = glm::vec3{material->emission_.data[0], material->emission_.data[1], material->emission_.data[2]};
  
  float pdf;
  const glm::vec3 omegaI = hemisphereSampling(shaderNormal, pdf);
  
  const RTCRayHitIor rayHitNew = generateRay(worldPos, omegaI);
  
  const glm::vec4 reflColor = traceRay(rayHitNew, depth - 1);
  
  const glm::vec3 diffuse = getDiffuseColor(material, tex_coord);
  
  const glm::vec3 fR = diffuse * glm::vec3(1. / M_PI);

//  float shadowVal = shadow(worldPos, lightDir, glm::l2Norm(light_->getPosition()));

//  emmision += glm::vec3(reflColor.x, reflColor.y, reflColor.z) * diffuse;
  
  //return glm::vec4(emmision.r, emmision.g, emmision.b, 1);
  
  glm::vec3 finalColor = emmision +
                         glm::vec3(reflColor.x, reflColor.y, reflColor.z) *
                         diffuse *
                         (glm::dot(shaderNormal, omegaI) / pdf);
  
  /*return glm::vec4(
      glm::vec3(reflColor.x, reflColor.y, reflColor.z) *
      glm::dot(shaderNormal, omegaI) *
      diffuse,
      1);*/
  
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