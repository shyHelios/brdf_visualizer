/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "brdfshader.h"


BRDFShader::BRDFShader(const char *vertex, const char *fragment) : Shader(vertex, fragment) {
  brdfUniformLocations_.init(shaderProgram);
}

void BRDFShader::use(Material *mtl) {
  Shader::use(mtl);
  
  setData(brdfUniformLocations_.IncidentVector, incidentVector_);
  setData(brdfUniformLocations_.PhongShininess, phongShininess_);
  
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

const glm::vec3 &BRDFShader::getIncidentVector() const {
  return incidentVector_;
}

void BRDFShader::setIncidentVector(const glm::vec3 &incidentVector) {
  incidentVector_ = incidentVector;
}

int BRDFShader::getPhongShininess() const {
  return phongShininess_;
}

void BRDFShader::setPhongShininess(int phongShininess) {
  phongShininess_ = phongShininess;
}

void BRDFShader::BRDFUniformLocations::init(int shaderProgram) {
  BasicUniformLocations::init(shaderProgram);
  MaterialUniformLocations::init(shaderProgram);
  
  IncidentVector = glGetUniformLocation(shaderProgram, "u_incidentVector");
  PhongShininess = glGetUniformLocation(shaderProgram, "u_phongShininess");
  
  if (IncidentVector == -1) { spdlog::warn("[SHADER]  IncidentVector not found"); }
  if (PhongShininess == -1) { spdlog::warn("[SHADER]  PhongShininess not found"); }
}

