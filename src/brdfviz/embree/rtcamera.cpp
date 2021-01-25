/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "rtcamera.h"


RTCamera::RTCamera(const int width, const int height, const float fov_y,
                   const glm::vec3 &view_from, const glm::vec3 view_at) {
  width_ = width;
  height_ = height;
  fov_y_ = fov_y;
  
  view_from_ = view_from;
  view_at_ = view_at;
  
  f_y_ = static_cast<float>(height_) / (2.f * tanf(fov_y_ / 2.f));
  
  M_c_w_ = getMCW();
}

RTCRay RTCamera::GenerateRay(const float x_i, const float y_i) {
  RTCRay ray = RTCRay();
  glm::vec3 dc_w = getRayDirection(x_i, y_i);
  
  ray.org_x = view_from_.x; // ray origin
  ray.org_y = view_from_.y;
  ray.org_z = view_from_.z;
  ray.tnear = FLT_MIN; // start of ray segment
  
  ray.dir_x = dc_w.x; // ray direction
  ray.dir_y = dc_w.y;
  ray.dir_z = dc_w.z;
  ray.time = 0.0f; // time of this ray for motion blur
  
  ray.tfar = FLT_MAX; // end of ray segment (set to hit distance)
  
  ray.mask = 0; // can be used to mask out some geometries for some rays
  ray.id = 0; // identify a ray inside a callback function
  ray.flags = 0; // reserved
  
  return ray;
}

int RTCamera::getWidth() const {
  return width_;
}

int RTCamera::getHeight() const {
  return height_;
}

float RTCamera::getFovY() const {
  return fov_y_;
}

const glm::vec3 &RTCamera::getViewFrom() const {
  
  
  return view_from_;
}

const glm::vec3 &RTCamera::getViewAt() const {
  return view_at_;
}

const glm::vec3 &RTCamera::getUp() const {
  return up_;
}

float RTCamera::getFY() const {
  return f_y_;
}

const glm::mat3 &RTCamera::getMCW() {
  glm::vec3 z_c = view_from_ - view_at_;
  z_c = glm::normalize(z_c);
  glm::vec3 x_c = glm::cross(up_, z_c);
  x_c = glm::normalize(x_c);
  glm::vec3 y_c = glm::cross(z_c, x_c);
  y_c = glm::normalize(y_c);
  
  M_c_w_ = glm::mat3(x_c, y_c, z_c);
  
  return M_c_w_;
}

const glm::vec3 RTCamera::getRayDirection(const float x_i, const float y_i) {
  glm::vec3 dc = glm::vec3((x_i - (static_cast<float>(width_) * 0.5f)),
                           (static_cast<float>(height_) * 0.5f) - y_i,
                           -f_y_);
  dc = glm::normalize(dc);
  return getMCW() * dc;
}