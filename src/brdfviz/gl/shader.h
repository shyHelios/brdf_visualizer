/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef SHADER_H
#define SHADER_H


#include "shaderloader.h"
#include "uniformlocations.h"
#include "material.h"
#include "camera.h"
#include "camerainfo.h"
#include "lightinfo.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Light;

class Shader : public ShaderLoader/*, std::enable_shared_from_this<Shader> */{
public:
  Shader(int shaderProgramStub);
  
  Shader(const char *vertex, const char *fragment);
  
  virtual ~Shader();
  
  virtual void use(const std::shared_ptr<Material> &mtl);
  
  virtual void setModelMatrix(glm::mat4 ModelMatrix);
  
  virtual void addCamera(const std::shared_ptr<Camera> &camera);
  
  virtual void addLight(const std::shared_ptr<Light> &light);
  
  virtual void addLight(const std::vector<std::shared_ptr<Light>> &light);
  
  virtual void notify(const CameraInfo &c);

//  virtual void notify(const LightInfo &l);

protected:
  
  template<typename Tdata>
  void setData(const std::string &name, const Tdata &value, const int overrideType = -1) const;
  
  template<typename Tdata>
  void setData(const int location, const Tdata &value, const int overrideType = -1) const;
  
  struct UniformLocations {
    
    virtual void init(int shaderProgram) = 0;
  };
  
  struct BasicUniformLocations : UniformLocations {
    int ModelTransform = -1; //Modelova matice
    int ViewTransform = -1; //Pohledova matice
    int ProjectionTransform = -1; //Projekcni matice
    int CameraPosition = -1; //Pozice kamery
    
    virtual void init(int shaderProgram) override;
  };
  
  struct MaterialUniformLocations : UniformLocations {
    //Diffuse / Albedo
    int MaterialDiffuseColor = -1;
    int MaterialDiffuseTexture = -1;
    int MaterialHasDiffuseTexture = -1;
    
    //Specular
    int MaterialSpecularColor = -1;
    int MaterialSpecularTexture = -1;
    int MaterialHasSpecularTexture = -1;
    
    //Normal
    int MaterialNormalTexture = -1;
    int MaterialHasNormalTexture = -1;
    
    int MaterialRoughness = -1;
    int MaterialRoughnessTexture = -1;
    int MaterialHasRoughnessTexture = -1;
    
    
    //Metalness
    int MaterialMetalness = -1;
    int MaterialMetalnessTexture = -1;
    int MaterialHasMetalnessTexture = -1;
    
    int MaterialShininess = -1;
    int MaterialIor = -1;
    
    
    virtual void init(int shaderProgram) override;
  };
  
  struct EnvironmentUniformLocations : UniformLocations {
    int environmentMap = -1;
    int GGXIntegrMap = -1;
    int irradianceMap = -1;
    int maxMip = -1;
    
    virtual void init(int shaderProgram) override;
  };
  
  struct LightUniformLocations : UniformLocations {
    int LightCount = -1;
    struct ShaderLightInfo {
      int position = -1;
      int ambient = -1;
      int diffuse = -1;
      int specular = -1;
      int constant = -1;
      int linear = -1;
      int quadratic = -1;
    };
    std::vector<ShaderLightInfo> lightInfo;
    
    virtual void init(int shaderProgram) override;
    
    void addLight(const std::shared_ptr<Light> &light, int shaderProgramId);
    
    void addLight(const std::vector<std::shared_ptr<Light>> &lights, int shaderProgramId);
  };
  
  CameraInfo camInfo;
  int id;
  static int counterID;
  static int currentShaderID;
  std::shared_ptr<Camera> camera_;
  std::vector<std::shared_ptr<Light>> lights_;
  
  void checkCompilation();
  
  int shaderProgram;
  glm::mat4 modelMatrix;
  glm::mat4 normalMatrix;
};

template<>
void Shader::setData<bool>(const std::string &name, const bool &value, const int overrideType) const;

template<>
void Shader::setData<int>(const std::string &name, const int &value, const int overrideType) const;

template<>
void Shader::setData<float>(const std::string &name, const float &value, const int overrideType) const;

template<>
void Shader::setData<double>(const std::string &name, const double &value, const int overrideType) const;

template<>
void Shader::setData<glm::vec3>(const std::string &name, const glm::vec3 &value, const int overrideType) const;

template<>
void Shader::setData<glm::vec4>(const std::string &name, const glm::vec4 &value, const int overrideType) const;

template<>
void Shader::setData<glm::mat4x4>(const std::string &name, const glm::mat4x4 &value, const int overrideType) const;

template<>
void Shader::setData<glm::mat3x3>(const std::string &name, const glm::mat3x3 &value, const int overrideType) const;

template<>
void Shader::setData<bool>(const int location, const bool &value, const int overrideType) const;

template<>
void Shader::setData<int>(const int location, const int &value, const int overrideType) const;

template<>
void Shader::setData<float>(const int location, const float &value, const int overrideType) const;

template<>
void Shader::setData<double>(const int location, const double &value, const int overrideType) const;

template<>
void Shader::setData<glm::vec3>(const int location, const glm::vec3 &value, const int overrideType) const;

template<>
void Shader::setData<glm::vec4>(const int location, const glm::vec4 &value, const int overrideType) const;

template<>
void Shader::setData<glm::mat4x4>(const int location, const glm::mat4x4 &value, const int overrideType) const;

template<>
void Shader::setData<glm::mat3x3>(const int location, const glm::mat3x3 &value, const int overrideType) const;

#endif //SHADER_H
