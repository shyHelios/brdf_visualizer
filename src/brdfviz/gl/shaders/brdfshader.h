/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef BRDFSHADER_H
#define BRDFSHADER_H

#include "gl/shader.h"

class BRDFShader : public Shader {
public:
  BRDFShader(const char *vertex = "./Shaders/brdf/VertexShader.glsl", const char *fragment = "./Shaders/brdf/FragmentShader.glsl");
  
  virtual void use(Material *mtl) override;
  
  const glm::vec3 &getIncidentVector() const;
  
  void setIncidentVector(const glm::vec3 &incidentVector);
  
  int getPhongShininess() const;
  
  void setPhongShininess(int phongShininess);

private:
  struct BRDFUniformLocations : BasicUniformLocations, MaterialUniformLocations {
    virtual void init(int shaderProgram) override;
    
    int IncidentVector = -1;
    int PhongShininess = -1;
  };
  
  BRDFUniformLocations brdfUniformLocations_;
  glm::vec3 incidentVector_;
  int phongShininess_ = 32;
};


#endif //BRDFSHADER_H
