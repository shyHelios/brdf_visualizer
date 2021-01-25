/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef MATH_H
#define MATH_H

template<class T>
inline T sqr(const T x) {
  return x * x;
}

inline float deg2rad(const float x) {
  return x * float(M_PI) / 180.0f;
}

inline float clamp(const float v, const float lo, const float hi) {
  return (v < lo) ? lo : (hi < v) ? hi : v;
}

#endif //MATH_H
