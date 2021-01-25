/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTSPHERICALMAP_H
#define RTSPHERICALMAP_H

class RTSphericalMap {
public:
  explicit RTSphericalMap(const std::string &filename);
  
  glm::vec3 texel(const glm::vec3 &pos);

private:
  std::unique_ptr<Texture3f> texture_;
};


#endif //RTSPHERICALMAP_H
