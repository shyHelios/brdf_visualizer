/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "brdfshader.h"

const std::pair<const char *, BRDFShader::BRDF> BRDFShader::brdfArray[static_cast<int>(BRDF::CountBrdf)] = {
    {"Phong",                    BRDF::Phong},
    {"BlinnPhong",               BRDF::BlinnPhong},
    {"Physically correct Phong", BRDF::PhongPhysCorrect},
    {"Lambert",                  BRDF::Lambert},
    {"Torrance-Sparrow",         BRDF::TorranceSparrow},
    {"Oren-Nayar",               BRDF::OrenNayar},
    {"Mirror",                   BRDF::Mirror},
};

BRDFShader::BRDFShader(const char *vertex, const char *fragment) : Shader(vertex, fragment) {
  brdfUniformLocations_.init(shaderProgram);
}

void BRDFShader::use(const std::shared_ptr<Material> &mtl) {
  using Phong = PhongUniformLocationsPack;
  using TorranceSparrow = TorranceSparrowUniformLocationsPack;
  using Lambert = LambertUniformLocationsPack;
  using OrenNayar = OrenNayarUniformLocationsPack;
  
  Shader::use(mtl);
  
  setData(brdfUniformLocations_.incidentVector.getUniformLocation(), brdfUniformLocations_.incidentVector.getData());
  
  // Phong, Blinn Phong
  setData(brdfUniformLocations_.Phong::shininess.getUniformLocation(),
          brdfUniformLocations_.Phong::shininess.getData());
  setData(brdfUniformLocations_.Phong::specular.getUniformLocation(),
          brdfUniformLocations_.Phong::specular.getData());
  setData(brdfUniformLocations_.Phong::diffuse.getUniformLocation(),
          brdfUniformLocations_.Phong::diffuse.getData());
  
  // Torrance Sparrow
  setData(brdfUniformLocations_.TorranceSparrow::roughness.getUniformLocation(),
          brdfUniformLocations_.TorranceSparrow::roughness.getData());
  setData(brdfUniformLocations_.TorranceSparrow::f0.getUniformLocation(),
          brdfUniformLocations_.TorranceSparrow::f0.getData());
  
  // Lambert
  setData(brdfUniformLocations_.Lambert::reflectance.getUniformLocation(),
          brdfUniformLocations_.Lambert::reflectance.getData());
  
  // Oren Nayar
  setData(brdfUniformLocations_.OrenNayar::roughness.getUniformLocation(),
          brdfUniformLocations_.OrenNayar::roughness.getData());
  setData(brdfUniformLocations_.OrenNayar::reflectance.getUniformLocation(),
          brdfUniformLocations_.OrenNayar::reflectance.getData());
  
  
  setData(brdfUniformLocations_.Brdf, static_cast<int>(currentBrdfIdx));
  
  setData(brdfUniformLocations_.ModelTransform, modelMatrix);
  setData(brdfUniformLocations_.ProjectionTransform, camInfo.projectionMatrix);
  setData(brdfUniformLocations_.ViewTransform, camInfo.viewMatrix);
  for (int i = 0; i < NO_TEXTURES; i++) {
    auto *texture = mtl->texture(i);
    if (texture != nullptr) {
      glActiveTexture(texture->getTextureUnit());
      glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    }
    
    if (i == static_cast<int>(TextureSlot::diffuseSlot)) {
      if (texture != nullptr) {
        setData(brdfUniformLocations_.MaterialHasDiffuseTexture, 1.f);
        setData(brdfUniformLocations_.MaterialDiffuseTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(brdfUniformLocations_.MaterialHasDiffuseTexture, 0.f);
      }
    }
    if (i == static_cast<int>(TextureSlot::specularSlot)) {
      if (texture != nullptr) {
        setData(brdfUniformLocations_.MaterialHasSpecularTexture, 1.f);
        setData(brdfUniformLocations_.MaterialSpecularTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(brdfUniformLocations_.MaterialHasSpecularTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::normalSlot)) {
      if (texture != nullptr) {
        setData(brdfUniformLocations_.MaterialHasNormalTexture, 1.f);
        setData(brdfUniformLocations_.MaterialNormalTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(brdfUniformLocations_.MaterialHasNormalTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::roughnessSlot)) {
      if (texture != nullptr) {
        setData(brdfUniformLocations_.MaterialHasRoughnessTexture, 1.f);
        setData(brdfUniformLocations_.MaterialRoughnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(brdfUniformLocations_.MaterialHasRoughnessTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::metalicnessSlot)) {
      if (texture != nullptr) {
        setData(brdfUniformLocations_.MaterialHasMetalnessTexture, 1.f);
        setData(brdfUniformLocations_.MaterialMetalnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(brdfUniformLocations_.MaterialHasMetalnessTexture, 0.0f);
      }
    }
  }
  
}

BRDFShader::BRDFUniformLocations &BRDFShader::getBrdfUniformLocations() {
  return brdfUniformLocations_;
}

bool BRDFShader::imguiSelectionGetter(void *data, int idx, const char **out_str) {
  *out_str = reinterpret_cast<std::pair<const char *, BRDF> *>(data)[idx].first;
  return true;
}

void BRDFShader::BRDFUniformLocations::init(int shaderProgram) {
  BasicUniformLocations::init(shaderProgram);
  MaterialUniformLocations::init(shaderProgram);
  PhongUniformLocationsPack::init(shaderProgram);
  TorranceSparrowUniformLocationsPack::init(shaderProgram);
  LambertUniformLocationsPack::init(shaderProgram);
  OrenNayarUniformLocationsPack::init(shaderProgram);
  
  incidentVector.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_incidentVector");
  Brdf = glGetUniformLocation(shaderProgram, "u_brdf");
  
  if (incidentVector.getUniformLocation() == -1) { spdlog::warn("[SHADER]  IncidentVector not found"); }
  if (Brdf == -1) { spdlog::warn("[SHADER]  Brdf not found"); }
}

void BRDFShader::PhongUniformLocationsPack::init(int shaderProgram) {
  shininess.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_phongShininess");
  diffuse.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_phongDiffuse");
  specular.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_phongSpecular");
  
  shininess.getData() = 32;
  diffuse.getData() = 0.5;
  specular.getData() = 0.5;
  
  
  if (shininess.getUniformLocation() == -1) { spdlog::warn("[SHADER]  u_phongShininess not found"); }
  if (specular.getUniformLocation() == -1) { spdlog::warn("[SHADER]  PhongShininess not found"); }
  if (diffuse.getUniformLocation() == -1) { spdlog::warn("[SHADER]  PhongShininess not found"); }
}

void BRDFShader::TorranceSparrowUniformLocationsPack::init(int shaderProgram) {
  roughness.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_roughness");
  f0.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_f0");
  
  roughness.getData() = 0.1;
  f0.getData() = 0.1;
  
  if (roughness.getUniformLocation() == -1) { spdlog::warn("[SHADER]  roughness not found"); }
  if (f0.getUniformLocation() == -1) { spdlog::warn("[SHADER]  f0 not found"); }
}

void BRDFShader::LambertUniformLocationsPack::init(int shaderProgram) {
  reflectance.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_lambertReflectance");
  reflectance.getData() = 0.5;
  
  if (reflectance.getUniformLocation() == -1) { spdlog::warn("[SHADER] u_lambertReflectance not found"); }
}

void BRDFShader::OrenNayarUniformLocationsPack::init(int shaderProgram) {
  reflectance.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_orenNayarReflectance");
  roughness.getUniformLocation() = glGetUniformLocation(shaderProgram, "u_orenNayarRoughness");
  
  reflectance.getData() = 0.5;
  roughness.getData() = 0.1;
  
  if (reflectance.getUniformLocation() == -1) { spdlog::warn("[SHADER] u_orenNayarReflectance not found"); }
  if (roughness.getUniformLocation() == -1) { spdlog::warn("[SHADER]  u_orenNayarRoughness not found"); }
  
}
