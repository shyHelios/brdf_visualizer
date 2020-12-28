/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "normalshader.h"

NormalShader::NormalShader(const char *vertex, const char *fragment) : Shader(vertex, fragment) {
  normalUniformLocations_.init(shaderProgram);
}

void NormalShader::use(Material *mtl) {
  Shader::use(mtl);
  
  setData(normalUniformLocations_.ModelTransform, modelMatrix);
  setData(normalUniformLocations_.ProjectionTransform, camInfo.projectionMatrix);
  setData(normalUniformLocations_.ViewTransform, camInfo.viewMatrix);
  for (int i = 0; i < NO_TEXTURES; i++) {
    auto *texture = mtl->texture(i);
    if (texture != nullptr) {
      glActiveTexture(texture->getTextureUnit());
      glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    }
    
    if (i == static_cast<int>(TextureSlot::diffuseSlot)) {
      if (texture != nullptr) {
        setData(normalUniformLocations_.MaterialHasDiffuseTexture, 1.f);
        setData(normalUniformLocations_.MaterialDiffuseTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(normalUniformLocations_.MaterialHasDiffuseTexture, 0.f);
      }
    }
    if (i == static_cast<int>(TextureSlot::specularSlot)) {
      if (texture != nullptr) {
        setData(normalUniformLocations_.MaterialHasSpecularTexture, 1.f);
        setData(normalUniformLocations_.MaterialSpecularTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(normalUniformLocations_.MaterialHasSpecularTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::normalSlot)) {
      if (texture != nullptr) {
        setData(normalUniformLocations_.MaterialHasNormalTexture, 1.f);
        setData(normalUniformLocations_.MaterialNormalTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(normalUniformLocations_.MaterialHasNormalTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::roughnessSlot)) {
      if (texture != nullptr) {
        setData(normalUniformLocations_.MaterialHasRoughnessTexture, 1.f);
        setData(normalUniformLocations_.MaterialRoughnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(normalUniformLocations_.MaterialHasRoughnessTexture, 0.0f);
      }
    }
    if (i == static_cast<int>(TextureSlot::metalicnessSlot)) {
      if (texture != nullptr) {
        setData(normalUniformLocations_.MaterialHasMetalnessTexture, 1.f);
        setData(normalUniformLocations_.MaterialMetalnessTexture, (int) (texture->getTextureUnit() - GL_TEXTURE0), GL_SAMPLER_2D);
      } else {
        setData(normalUniformLocations_.MaterialHasMetalnessTexture, 0.0f);
      }
    }
  }
  
}

void NormalShader::NormalUniformLocations::init(int shaderProgram) {
  BasicUniformLocations::init(shaderProgram);
  MaterialUniformLocations::init(shaderProgram);
}