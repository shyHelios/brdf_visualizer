/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCRAYHITIOR_H
#define RTCRAYHITIOR_H

#include <embree3/rtcore.h>
#include <gl/material.h>


struct RTCRayHitIor : RTCRayHit {
  float ior = IOR_AIR;
  int mathObjIndex = -1;
  
  inline bool collided() const { return hit.geomID != RTC_INVALID_GEOMETRY_ID; }
};


#endif //RTCRAYHITIOR_H
