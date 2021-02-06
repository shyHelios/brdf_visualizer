/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>

#include "samplervisualizerobject.h"
#include "vertex.h"
#include "linevertexbufferobject.h"
#include <common/utils/math.h>

SamplerVisualizerObject::SamplerVisualizerObject(const std::shared_ptr<VertexBufferObject> &innerObject, const std::shared_ptr<Shader> &shader,
                                                 const std::shared_ptr<ObjectTransformation> &transformation, const std::shared_ptr<Material> &material)
    : Object(innerObject, shader, transformation, material) {
  sampler_ = std::make_unique<HemisphereSampler>();
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
  const double incrementStep = 1. / static_cast<double>(resolution);
  const std::vector<unsigned int> indices = {0, 1};
  
  float pdf = 0;
  for (int y = 0; y < resolution; y++) {
    for (int x = 0; x < resolution; x++) {
      float u = x * incrementStep;
      float v = y * incrementStep;
      
      const auto &sample = sampler_->sampleImpl(u, v, normal, pdf);
      const std::vector<Vertex> vertices = {glm::vec3(0, 0, 0), sample};
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
  for (int y = 0; y < resolution; y++) {
    for (int x = 0; x < resolution; x++) {
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
