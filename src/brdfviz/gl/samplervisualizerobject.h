/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SAMPLERVISUALIZEROBJECT_H
#define SAMPLERVISUALIZEROBJECT_H


#include "object.h"
#include <common/sampler.h>

class SamplerVisualizerObject : public Object {
public:
  SamplerVisualizerObject(glm::vec3 &normalRef,
                          glm::vec3 &incidentVectorRef,
                          glm::vec3 &reflectVectorRef,
                          const std::shared_ptr<VertexBufferObject> &innerObject = nullptr,
                          const std::shared_ptr<Shader> &shader = nullptr,
                          const std::shared_ptr<ObjectTransformation> &transformation = nullptr,
                          const std::shared_ptr<Material> &material = nullptr);
  
  
  void update();
  
  virtual void draw(const bool geometry) override;
  
  const std::shared_ptr<Sampler> &getSampler() const;
  
  void setSampler(const std::shared_ptr<Sampler> &sampler);
  
  glm::vec3 &normal;
  glm::vec3 &incidentVector;
  glm::vec3 &reflectVector;
  
  int &getResolution();

private:
  void recreate();
  
  void recompute();
  
  std::unique_ptr<Object> &at(int x, int y);
  
  std::shared_ptr<Sampler> sampler_;
  
  int resolution = 30;
  std::vector<std::unique_ptr<Object>> objects_;
};


#endif //SAMPLERVISUALIZEROBJECT_H
