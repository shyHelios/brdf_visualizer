/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "pathtracerhelper.h"

PathTracerHelper::PathTracerHelper(int height, int width) : width_(width), height_(height), traces_(0) {
  pixelsSum_.resize(width * height);
  resetTraces();
}

glm::vec4 PathTracerHelper::getPixel(int row, int col) {
  const int offset = (row * width_ + col);
  return pixelsSum_.at(offset);
}

void PathTracerHelper::setPixel(int row, int col, const glm::vec4 &pix) {
  const int offset = (row * width_ + col);
  pixelsSum_.at(offset) += pix;
}

glm::vec4 PathTracerHelper::getInterpolatedPixel(int row, int col) {
  glm::vec4 pix = getPixel(row, col);
  return pix / glm::vec4(pix.w);
}

void PathTracerHelper::incrementTraces() {
  traces_++;
}

void PathTracerHelper::resetTraces() {
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      const int offset = (row * width_ + col);
      pixelsSum_.at(offset) = glm::vec4(0, 0, 0, 0);
    }
  }
}

int PathTracerHelper::getTracesCount() {
  float max = 0;
  for (int row = 0; row < height_; row++) {
    for (int col = 0; col < width_; col++) {
      const int offset = (row * width_ + col);
      max = pixelsSum_[offset].w > max ? pixelsSum_[offset].w : max;
    }
  }
  return static_cast<int>(max);
}