/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef BRDFSHADER_H
#define BRDFSHADER_H

#include "gl/shader.h"
#include "gl/uniformlocationpair.h"

class BRDFShader : public Shader {
public:
  BRDFShader(const char *vertex = "./Shaders/brdf/VertexShader.glsl", const char *fragment = "./Shaders/brdf/FragmentShader.glsl");
  
  virtual void use(const std::shared_ptr<Material> &mtl) override;
  
  enum class BRDF : int {
    Phong = 0,
    BlinnPhong,
    TorranceSparrow,
    CountBrdf
  };
  
  struct PhongUniformLocationsPack : UniformLocations {
    virtual void init(int shaderProgram) override;
    
    UniformLocationPair<int> shininess;
  };
  
  struct TorranceSparrowUniformLocationsPack : UniformLocations {
    virtual void init(int shaderProgram) override;
    
    UniformLocationPair<float> roughness;
    UniformLocationPair<float> f0;
  };
  
  struct BRDFUniformLocations :
      BasicUniformLocations,
      MaterialUniformLocations,
      PhongUniformLocationsPack,
      TorranceSparrowUniformLocationsPack {
    virtual void init(int shaderProgram) override;
    
    UniformLocationPair<glm::vec3> incidentVector;
    int Brdf = -1;
  };
  
  BRDFUniformLocations &getBrdfUniformLocations();
  
  static const std::pair<const char *, BRDF> brdfArray[static_cast<int>(BRDF::CountBrdf)];
  
  BRDF currentBrdfIdx = BRDF::Phong;
  
  static bool imguiSelectionGetter(void *data, int idx, const char **out_str);


private:
  BRDFUniformLocations brdfUniformLocations_;
};


#endif //BRDFSHADER_H
