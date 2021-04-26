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
  
  glm::vec3 downsampledTexel(const glm::vec3 &pos);
  
  glm::vec3 sample(float &pdf);

private:
  
  void computeCdf();
  
  std::unique_ptr<Texture3f> texture_;
  std::unique_ptr<Texture3f> downsampledTexture_;
  std::vector<float> downsampledPdf_;
  std::vector<float> downsampledCdf_;
  std::vector<float> marginalCdf_;
};


#endif //RTSPHERICALMAP_H
