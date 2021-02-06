/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SAMPLER_H
#define SAMPLER_H


class Sampler {
public:
  friend class SamplerVisualizerObject;
  
  glm::vec3 sample(const glm::vec3 &normal, float &pdf) const;

private:
  virtual glm::vec3 sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const = 0;
};

class HemisphereSampler : public Sampler {
private:
  virtual glm::vec3 sampleImpl(const float randomU, const float randomV, const glm::vec3 &normal, float &pdf) const override;
};


#endif //SAMPLER_H
