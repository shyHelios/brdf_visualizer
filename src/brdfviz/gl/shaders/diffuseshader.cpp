/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "diffuseshader.h"

DiffuseShader::DiffuseShader(const char *vertex, const char *fragment) : Shader(vertex, fragment) {
  diffuseUniformLocations_.init(shaderProgram);
}

void DiffuseShader::use(const std::shared_ptr<Material> &mtl) {
  Shader::use(mtl);
  
  setData(diffuseUniformLocations_.ModelTransform, modelMatrix);
  setData(diffuseUniformLocations_.ProjectionTransform, camInfo.projectionMatrix);
  setData(diffuseUniformLocations_.ViewTransform, camInfo.viewMatrix);
  
  for (int i = 0; i < NO_TEXTURES; i++) {
    auto *texture = mtl->texture(i);
    if (texture != nullptr) {
      glActiveTexture(texture->getTextureUnit());
      glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    }
    
    if (i == static_cast<int>(TextureSlot::diffuseSlot)) {
      if (texture != nullptr) {
        setData(diffuseUniformLocations_.MaterialHasDiffuseTexture, 1.f);
        setData(diffuseUniformLocations_.MaterialDiffuseTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(diffuseUniformLocations_.MaterialHasDiffuseTexture, 0.f);
      }
    }
    if (i == static_cast<int>(TextureSlot::specularSlot)) {
      if (texture != nullptr) {
        setData(diffuseUniformLocations_.MaterialHasSpecularTexture, 1.f);
        setData(diffuseUniformLocations_.MaterialSpecularTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(diffuseUniformLocations_.MaterialHasSpecularTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::normalSlot)) {
      if (texture != nullptr) {
        setData(diffuseUniformLocations_.MaterialHasNormalTexture, 1.f);
        setData(diffuseUniformLocations_.MaterialNormalTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(diffuseUniformLocations_.MaterialHasNormalTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::roughnessSlot)) {
      if (texture != nullptr) {
        setData(diffuseUniformLocations_.MaterialHasRoughnessTexture, 1.f);
        setData(diffuseUniformLocations_.MaterialRoughnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(diffuseUniformLocations_.MaterialHasRoughnessTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::metalicnessSlot)) {
      if (texture != nullptr) {
        setData(diffuseUniformLocations_.MaterialHasMetalnessTexture, 1.f);
        setData(diffuseUniformLocations_.MaterialMetalnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(diffuseUniformLocations_.MaterialHasMetalnessTexture, 0.0f);
      }
    }
  }
  
  setData(diffuseUniformLocations_.MaterialDiffuseColor, ColorToGlm(mtl->diffuse_));
}

void DiffuseShader::DiffuseUniformLocations::init(int shaderProgram) {
  BasicUniformLocations::init(shaderProgram);
  MaterialUniformLocations::init(shaderProgram);
}
