/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTCSHADINGTYPE_H
#define RTCSHADINGTYPE_H

enum class RTCShadingType {
  None = -1,
  Lambert = 3,
  Glass = 4,
  PathTracing = 5,
  Mirror = 6,
  Phong = 7,
  Normals = -2,
  TexCoords = -3
};

inline bool isShadingType(int type) {
  if (type < 0) return false;
  
  if (type == static_cast<int>(RTCShadingType::Lambert))
    return true;
  
  if (type == static_cast<int>(RTCShadingType::Glass))
    return true;
  
  if (type == static_cast<int>(RTCShadingType::PathTracing))
    return true;
  
  if (type == static_cast<int>(RTCShadingType::Mirror))
    return true;
  
  if (type == static_cast<int>(RTCShadingType::Phong))
    return true;
  
  return false;
}


#endif //RTCSHADINGTYPE_H
