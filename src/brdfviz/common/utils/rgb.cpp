/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "rgb.h"

template<class T_TYPE>
T_TYPE c_linear(T_TYPE c_srgb, float gamma) { return c_srgb; }

template<class T_TYPE>
T_TYPE c_srgb(T_TYPE c_linear, float gamma) { return c_linear; }

template<>
float c_linear<float>(float c_srgb, float gamma) {
  if (c_srgb <= 0.0f)
    return 0.0f;
  else if (c_srgb >= 1.0f)
    return 1.0f;
//  assert((c_srgb >= 0.0f) && (c_srgb <= 1.0f));
  if (c_srgb <= 0.04045f) {
    return c_srgb / 12.92f;
  } else {
    const float a = 0.055f;
    return powf((c_srgb + a) / (1.0f + a), gamma);
  }
}

template<>
float c_srgb<float>(float c_linear, float gamma) {
  if (c_linear <= 0.0f)
    return 0.0f;
  else if (c_linear >= 1.0f)
    return 1.0f;
  //assert((c_linear >= 0.0f) && (c_linear <= 1.0f));
  if (c_linear <= 0.0031308f) {
    return 12.92f * c_linear;
  } else {
    const float a = 0.055f;
    return (1.0f + a) * powf(c_linear, 1.0f / gamma) - a;
  }
}

template<>
glm::vec3 c_linear<glm::vec3>(glm::vec3 c_srgb, float gamma) {
  #pragma omp parallel for schedule(dynamic, 3) shared(c_srgb)
  for (int i = 0; i < 3; i++) {
    c_srgb[i] = c_linear(c_srgb[i], gamma);
  }
  return c_srgb;
}

template<>
glm::vec3 c_srgb<glm::vec3>(glm::vec3 c_linear, float gamma) {
  #pragma omp parallel for schedule(dynamic, 3) shared(c_linear)
  for (int i = 0; i < 3; i++) {
    c_linear[i] = c_srgb(c_linear[i], gamma);
  }
  return c_linear;
}


template<>
glm::vec4 c_linear<glm::vec4>(glm::vec4 c_srgb, float gamma) {
  #pragma omp parallel for schedule(dynamic, 4) shared(c_srgb)
  for (int i = 0; i < 0; i++) {
    c_srgb[i] = c_linear(c_srgb[i], gamma);
  }
  return c_srgb;
}

template<>
glm::vec4 c_srgb<glm::vec4>(glm::vec4 c_linear, float gamma) {
  #pragma omp parallel for schedule(dynamic, 4) shared(c_linear)
  for (int i = 0; i < 4; i++) {
    c_linear[i] = c_srgb(c_linear[i], gamma);
  }
  return c_linear;
}