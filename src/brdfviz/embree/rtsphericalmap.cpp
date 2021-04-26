/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/math.h>
#include <gl/texture.h>
#include <common/utils/rng.h>
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
  computeCdf();
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

void RTSphericalMap::computeCdf() {
  const size_t height = downsampledTexture_->height();
  const size_t width = downsampledTexture_->width();

//   y * width + x
  downsampledPdf_.resize(height * width);
  downsampledCdf_.resize(height * width);
  marginalCdf_.resize(width);
  
  float sum = 0;
  
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      sum += downsampledTexture_->pixel(x, y).toGrayscale();
    }
  }
//  sum /= (width * height);
  
  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      downsampledPdf_[y * width + x] += downsampledTexture_->pixel(x, y).toGrayscale() / sum;
    }
  }
  
  float xPdf = 0;
  for (size_t x = 0; x < width; x++) {
    float yPdf = 0;
    for (size_t y = 0; y < height; y++) {
      yPdf += downsampledPdf_[y * width + x];
      downsampledCdf_[y * width + x] = yPdf;
    }
    xPdf += yPdf;
    marginalCdf_[x] = xPdf;
  }
  
  sum = 0;
  for (const float pdf : downsampledPdf_) {
    sum += pdf;
  }
//  spdlog::info("[SPHERE MAP] pdf sum: {}", sum);
}

glm::vec3 RTSphericalMap::sample(float &pdf) {
  const float u = rng();
  const float v = rng();
  const size_t height = downsampledTexture_->height();
  const size_t width = downsampledTexture_->width();
  
  float marginalSum = 0;
  for (const float marginal : marginalCdf_) {
    marginalSum += marginal;
  }
  size_t colIdx;
  const float sampleU = u * marginalSum;
  float currentSum = 0;
  for (colIdx = 0; colIdx < marginalCdf_.size(); colIdx++) {
    currentSum += marginalCdf_.at(colIdx);
    if (sampleU <= currentSum) {
      break;
    }
  }
  
  float cdfSum = 0;
  for (size_t y = 0; y < height; y++) {
    cdfSum += downsampledCdf_[y * width + colIdx];
  }
  const float sampleV = v * cdfSum;
  
  size_t rowIdx;
  currentSum = 0;
  for (rowIdx = 0; rowIdx < height; rowIdx++) {
    currentSum += downsampledCdf_[rowIdx * width + colIdx];
    if (sampleV <= currentSum) {
      break;
    }
  }
  
  const glm::vec3 sampledColor = colorToGlm(downsampledTexture_->pixel(colIdx, rowIdx));
  pdf = downsampledPdf_[rowIdx * width + colIdx];
  return sampledColor;
}
