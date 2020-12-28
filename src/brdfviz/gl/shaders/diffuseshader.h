/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef DIFFUSESHADER_H
#define DIFFUSESHADER_H


#include "gl/shader.h"


class DiffuseShader : public Shader {
public:
  DiffuseShader(const char *vertex = "./Shaders/Default/VertexShader.glsl", const char *fragment = "./Shaders/Default/FragmentShader.glsl");
  
  virtual void use(Material *mtl) override;

private:
  struct DiffuseUniformLocations : BasicUniformLocations, MaterialUniformLocations {
    virtual void init(int shaderProgram) override;
  };
  
  DiffuseUniformLocations diffuseUniformLocations_;
  
};

#endif //DIFFUSESHADER_H
