/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/math.h>
#include <gl/texture.h>
#include "rtsphericalmap.h"

RTSphericalMap::RTSphericalMap(const std::string &filename) {
  texture_ = std::make_unique<Texture3f>(filename.c_str());
  downsampledTexture_ = std::make_unique<Texture3f>(128 / 2, 64 / 2);
  
  const float scaleW = texture_->width() / downsampledTexture_->width();
  const float scaleH = texture_->height() / downsampledTexture_->height();
  const float overallScale = scaleW * scaleH;
  
  auto &downsampledData = downsampledTexture_->getData();
  auto texData = texture_->getData();
  
  for (int x = 0; x < texture_->width(); x++) {
    for (int y = 0; y < texture_->height(); y++) {
      auto pix = texData[size_t(x) + size_t(y) * size_t(texture_->width())];
      
      for (int i = 0; i < pix.data.size(); i++) {
        pix.data.at(i) = pix.data.at(i) / overallScale;
      }
      
      downsampledData[size_t(x / scaleW) + size_t(y / scaleH) * size_t(downsampledTexture_->width())] += pix;
    }
  }
}

glm::vec3 RTSphericalMap::texel(const glm::vec3 &pos) {
  float theta = acosf(clamp(pos.z, -1.f, 1.f));
  float phi = atan2f(pos.y, pos.x) + static_cast<float>(M_PI);
  
  float u = 1.0f - phi * 0.5f * static_cast<float>(M_1_PI);
  float v = theta * static_cast<float>(M_1_PI);

//  return colorToGlm(downsampledTexture_->texel(u, v));
  return colorToGlm(texture_->texel(u, v));
}

glm::vec3 RTSphericalMap::downsampledTexel(const glm::vec3 &pos) {
  float theta = acosf(clamp(pos.z, -1.f, 1.f));
  float phi = atan2f(pos.y, pos.x) + static_cast<float>(M_PI);
  
  float u = 1.0f - phi * 0.5f * static_cast<float>(M_1_PI);
  float v = theta * static_cast<float>(M_1_PI);
  
  return colorToGlm(downsampledTexture_->texel(u, v));
}
