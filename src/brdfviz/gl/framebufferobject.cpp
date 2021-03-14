/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "framebufferobject.h"

FrameBufferObject::FrameBufferObject(uint16_t width, uint16_t height) : width_(width), height_(height) {
  glCall(glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &maxSize_[0]));
  generate();
}

FrameBufferObject::~FrameBufferObject() {
  glDeleteFramebuffers(1, &frameBufferID);
  glDeleteTextures(1, &frameBufferColorID);
  glDeleteTextures(1, &frameBufferDepthID);
}

void FrameBufferObject::generate() {
  // if framebuffer already exists, delete it with all attachments
  if (frameBufferID > 0) {
    glCall(glDeleteFramebuffers(1, &frameBufferID));
    glCall(glDeleteTextures(1, &frameBufferColorID));
    glCall(glDeleteTextures(1, &frameBufferDepthID));
  }
  // Create and bind framebuffer
  glCall(glCreateFramebuffers(1, &frameBufferID));
  glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID));
  
  // COLOR ATTACHMENT
  
  // Create texture for color attachment and bind it
  glCall(glCreateTextures(GL_TEXTURE_2D, 1, &frameBufferColorID));
  glCall(glBindTexture(GL_TEXTURE_2D, frameBufferColorID));
  glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, realWidth_, realHeight_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
//  glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferColorID, 0));
  
  // DEPTH ATTACHMENT
  
  // Create texture for depth attachment and bind it
  glCall(glCreateTextures(GL_TEXTURE_2D, 1, &frameBufferDepthID));
  glCall(glBindTexture(GL_TEXTURE_2D, frameBufferDepthID));
  glCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, realWidth_, realHeight_));
//  glCall(glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width_, height_));
  glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, frameBufferDepthID, 0));
  
  assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
  unbind();
  
}

void FrameBufferObject::bind() const {
  glCall(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID));
  glCall(glViewport(0, 0, width_, height_));
}

void FrameBufferObject::unbind() const {
  glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

bool FrameBufferObject::resize(uint16_t width, uint16_t height) {
  if (width <= 0 || height <= 0 || width > maxSize_[0] || height > maxSize_[1]) {
    spdlog::warn("[FBO] attempt to resize FBO to invalid size. Requested: [{},{}], max: [{},{}]", width, height, maxSize_[0], maxSize_[1]);
    return false;
  }
  
  if (width == width_ && height == height_) {
    return true;
  }
  
  spdlog::info("[FBO] resizing to  [{},{}]", width, height);
  width_ = width;
  height_ = height;
  return true;
}

uint32_t FrameBufferObject::getWidth() const {
  return width_;
}

uint32_t FrameBufferObject::getHeight() const {
  return height_;
}

uint32_t FrameBufferObject::getFrameBufferId() const {
  return frameBufferID;
}

uint32_t FrameBufferObject::getFrameBufferColorId() const {
  return frameBufferColorID;
}

uint32_t FrameBufferObject::getFrameBufferDepthId() const {
  return frameBufferDepthID;
}

void FrameBufferObject::saveScreen() const {
  bind();
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);
  
  std::stringstream ss;
  ss << "out/";
  ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
  ss << ".png";
  
  BYTE *pixels = (BYTE *) malloc(width_ * height_ * 3);
  
  glReadPixels(0, 0, width_, height_, GL_BGR, GL_UNSIGNED_BYTE, pixels);
  
  FIBITMAP *image = FreeImage_ConvertFromRawBits(pixels, width_, height_, 3 * width_, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
  
  if (FreeImage_Save(FIF_PNG, image, ss.str().c_str(), 0))
    printf("Successfully saved!\n");
  else
    printf("Failed saving!\n");
  
  FreeImage_Unload(image);
  
  free(pixels);
  unbind();
}

uint32_t FrameBufferObject::getRealWidth() const {
  return realWidth_;
}

uint32_t FrameBufferObject::getRealHeight() const {
  return realHeight_;
}

glm::vec2 FrameBufferObject::getUV() const {
  return glm::vec2(static_cast<float>(width_) / static_cast<float>(realWidth_),
                   static_cast<float>(height_) / static_cast<float>(realHeight_));
}

float FrameBufferObject::getRatio() const {
  return static_cast<float>(width_) / static_cast<float>(height_);
}
