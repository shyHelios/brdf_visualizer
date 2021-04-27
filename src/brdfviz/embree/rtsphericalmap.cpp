/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <common/utils/math.h>
#include <gl/texture.h>
#include <common/utils/rng.h>
#include <common/utils/coordsystem.h>
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
  glm::vec2 spherical = cartesianToSpherical(pos);
  float theta = spherical.x;
  float phi = spherical.y;
  float u = 1.0f - (phi * 0.5f * static_cast<float>(M_1_PI));
//  float u = 1.0f - (phi / (2. * M_PI));
  float v = theta * static_cast<float>(M_1_PI);

//  theta = v * M_PI;
//  phi = (1. - u) * (2 * M_PI);
//
//  glm::vec3 cartesian = sphericalToCartesian(theta, phi);
//  cartesian.x *= -1.;
//  cartesian.y *= -1.;
//
//  float dist = glm::distance(pos, cartesian);
//
//  if (dist > 0.001f) {
//    spdlog::warn("dist: {} [{},{},{}],  [{},{},{}]", dist, cartesian.x, cartesian.y, cartesian.z, pos.x, pos.y, pos.z);
//  }
//
  return colorToGlm(texture_->texel(u, v));
}

glm::vec3 RTSphericalMap::downsampledTexel(const glm::vec3 &pos) {
  glm::vec2 spherical = cartesianToSpherical(pos);
  float theta = spherical.x;
  float phi = spherical.y;
  float u = 1.0f - (phi * 0.5f * static_cast<float>(M_1_PI));
//  float u = 1.0f - (phi / (2. * M_PI));
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

glm::vec3 RTSphericalMap::sample(float &pdf, glm::vec3 &omegaI) {
  const float randU = rng();
  const float randV = rng();
  const size_t height = downsampledTexture_->height();
  const size_t width = downsampledTexture_->width();
  
  float marginalSum = 0;
  for (const float marginal : marginalCdf_) {
    marginalSum += marginal;
  }
  size_t colIdx;
  const float sampleU = randU * marginalSum;
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
  const float sampleV = randV * cdfSum;
  
  size_t rowIdx;
  currentSum = 0;
  for (rowIdx = 0; rowIdx < height; rowIdx++) {
    currentSum += downsampledCdf_[rowIdx * width + colIdx];
    if (sampleV <= currentSum) {
      break;
    }
  }
  
  float texU = static_cast<float>(colIdx) / static_cast<float>(width);
  float texV = static_cast<float>(rowIdx) / static_cast<float>(height);
  
  float theta = texV * M_PI;
  float phi = (1. - texU) * (2 * M_PI);
  
  glm::vec3 cartesian = sphericalToCartesian(theta, phi);
  cartesian.x *= -1.;
  cartesian.y *= -1.;
  
  omegaI = sphericalToCartesian(theta, phi);
  
  
  const glm::vec3 sampledColor = colorToGlm(downsampledTexture_->pixel(colIdx, rowIdx));
  pdf = downsampledPdf_[rowIdx * width + colIdx];
  return sampledColor;
}
