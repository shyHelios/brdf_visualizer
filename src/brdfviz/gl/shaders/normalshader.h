/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef NORMALSHADER_H
#define NORMALSHADER_H


#include "gl/shader.h"

class NormalShader : public Shader {
public:
  NormalShader(const char *vertex = "./Shaders/Normal/VertexShader.glsl", const char *fragment = "./Shaders/Normal/FragmentShader.glsl");
  
  virtual void use(Material *mtl) override;

private:
  struct NormalUniformLocations : BasicUniformLocations, MaterialUniformLocations {
    virtual void init(int shaderProgram) override;
  };
  
  NormalUniformLocations normalUniformLocations_;
};


#endif //NORMALSHADER_H
