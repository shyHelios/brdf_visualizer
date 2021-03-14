/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>

#include "samplervisualizerobject.h"
#include "vertex.h"
#include "linevertexbufferobject.h"
#include <common/utils/math.h>

SamplerVisualizerObject::SamplerVisualizerObject(glm::vec3 &normalRef,
                                                 glm::vec3 &incidentVectorRef,
                                                 glm::vec3 &reflectVectorRef,
                                                 const std::shared_ptr<Sampler> &sampler,
                                                 const std::shared_ptr<VertexBufferObject> &innerObject,
                                                 const std::shared_ptr<Shader> &shader,
                                                 const std::shared_ptr<ObjectTransformation> &transformation,
                                                 const std::shared_ptr<Material> &material)
    : Object(innerObject, shader, transformation, material),
      normal(normalRef),
      incidentVector(incidentVectorRef),
      reflectVector(reflectVectorRef),
      sampler_(sampler) {
  update();
}

void SamplerVisualizerObject::recreate() {
  objects_.reserve(sqr(resolution));
  
  const std::vector<Vertex> vertices = {glm::vec3(0, 0, 0), glm::vec3(0.5, 0.5, 0.5)};
  const std::vector<unsigned int> indices = {0, 1};
  
  auto incidentVectorVBO = std::make_shared<LineVertexBufferObject>(vertices, indices, 4);
  const int currentNumberOfObjects = objects_.size();
  const int requiredNumberOfObjects = sqr(resolution);
  const int missingObjectsCount = requiredNumberOfObjects - currentNumberOfObjects;
  for (int i = 0; i < missingObjectsCount; i++) {
    auto vbo = std::make_shared<LineVertexBufferObject>(vertices, indices, 2);
    objects_.emplace_back(std::make_unique<Object>(vbo, shader, transformation, material));
  }
}

std::unique_ptr<Object> &SamplerVisualizerObject::at(int x, int y) {
  assert(x >= 0);
  assert(y >= 0);
  
  assert(x < resolution);
  assert(y < resolution);
  
  const int offset = (y * resolution + x);
  return objects_.at(offset);
}

void SamplerVisualizerObject::recompute() {
  const float incrementStep = 1. / static_cast<float>(resolution);
  const std::vector<unsigned int> indices = {0, 1};
  
  float pdf = 0;
//  int idx = 0;
  for (int y = 0; y < resolution - 1; y++) {
    for (int x = 0; x < resolution - 1; x++) {
      float u = (x + 1) * incrementStep;
      float v = (y + 1) * incrementStep;
      
      auto sample = sampler_->sample(u, v, normal, reflectVector, pdf);
      if (multiplyByPdf) {
        sample *= pdf;
      }
      
      const std::vector<Vertex> vertices = {{glm::vec3(0, 0, 0), sample},
                                            {sample,             sample}};
      at(x, y)->innerObject->recreate(vertices, indices);
    }
  }
}

void SamplerVisualizerObject::update() {
  recreate();
  recompute();
}

void SamplerVisualizerObject::draw(const bool geometry) {
  if (!visible) return;
  for (int y = 0; y < resolution - 1; y++) {
    for (int x = 0; x < resolution - 1; x++) {
      const auto &obj = at(x, y);
      obj->draw(geometry);
    }
  }
}

const std::shared_ptr<Sampler> &SamplerVisualizerObject::getSampler() const {
  return sampler_;
}

void SamplerVisualizerObject::setSampler(const std::shared_ptr<Sampler> &sampler) {
  sampler_ = sampler;
}

int &SamplerVisualizerObject::getResolution() {
  return resolution;
}

bool &SamplerVisualizerObject::getMultiplyByPdf() {
  return multiplyByPdf;
}
