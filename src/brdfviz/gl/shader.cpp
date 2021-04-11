/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "shader.h"

#include <pch.h>
#include "shader.h"
#include "camera.h"
#include "light.h"

int Shader::counterID = 0;
int Shader::currentShaderID = -1;

///@brief Macro to enable or disable checking validity of shaders uniform variables
#define SHADERS_CHECKING 0

///@brief Macro to enable or disable throwing exceptions when invalid type passed to uniform
#define SHADERS_PEDANTIC_CHECKING 0

#if (NDEBUG || !SHADERS_CHECKING)
  #define glCheckLocation(x) (void)(x)
#else
#define glCheckLocation(x) {\
    if (!(x)){   \
        spdlog::warn("[SHADER] Uniform varible not set - file '{}' line {}", __FILE__, __LINE__); \
    }\
  }
#endif

const char *getGlenumName(GLenum type) {
  switch (type) {
    case GL_FLOAT : return "GL_FLOAT";
    case GL_FLOAT_VEC2 : return "GL_FLOAT_VEC2";
    case GL_FLOAT_VEC3 : return "GL_FLOAT_VEC3";
    case GL_FLOAT_VEC4 : return "GL_FLOAT_VEC4";
    case GL_DOUBLE : return "GL_DOUBLE";
    case GL_DOUBLE_VEC2 : return "GL_DOUBLE_VEC2";
    case GL_DOUBLE_VEC3 : return "GL_DOUBLE_VEC3";
    case GL_DOUBLE_VEC4 : return "GL_DOUBLE_VEC4";
    case GL_INT : return "GL_INT";
    case GL_INT_VEC2 : return "GL_INT_VEC2";
    case GL_INT_VEC3 : return "GL_INT_VEC3";
    case GL_INT_VEC4 : return "GL_INT_VEC4";
    case GL_UNSIGNED_INT : return "GL_UNSIGNED_INT";
    case GL_UNSIGNED_INT_VEC2 : return "GL_UNSIGNED_INT_VEC2";
    case GL_UNSIGNED_INT_VEC3 : return "GL_UNSIGNED_INT_VEC3";
    case GL_UNSIGNED_INT_VEC4 : return "GL_UNSIGNED_INT_VEC4";
    case GL_BOOL : return "GL_BOOL";
    case GL_BOOL_VEC2 : return "GL_BOOL_VEC2";
    case GL_BOOL_VEC3 : return "GL_BOOL_VEC3";
    case GL_BOOL_VEC4 : return "GL_BOOL_VEC4";
    case GL_FLOAT_MAT2 : return "GL_FLOAT_MAT2";
    case GL_FLOAT_MAT3 : return "GL_FLOAT_MAT3";
    case GL_FLOAT_MAT4 : return "GL_FLOAT_MAT4";
    case GL_FLOAT_MAT2x3 : return "GL_FLOAT_MAT2x3";
    case GL_FLOAT_MAT2x4 : return "GL_FLOAT_MAT2x4";
    case GL_FLOAT_MAT3x2 : return "GL_FLOAT_MAT3x2";
    case GL_FLOAT_MAT3x4 : return "GL_FLOAT_MAT3x4";
    case GL_FLOAT_MAT4x2 : return "GL_FLOAT_MAT4x2";
    case GL_FLOAT_MAT4x3 : return "GL_FLOAT_MAT4x3";
    case GL_DOUBLE_MAT2 : return "GL_DOUBLE_MAT2";
    case GL_DOUBLE_MAT3 : return "GL_DOUBLE_MAT3";
    case GL_DOUBLE_MAT4 : return "GL_DOUBLE_MAT4";
    case GL_DOUBLE_MAT2x3 : return "GL_DOUBLE_MAT2x3";
    case GL_DOUBLE_MAT2x4 : return "GL_DOUBLE_MAT2x4";
    case GL_DOUBLE_MAT3x2 : return "GL_DOUBLE_MAT3x2";
    case GL_DOUBLE_MAT3x4 : return "GL_DOUBLE_MAT3x4";
    case GL_DOUBLE_MAT4x2 : return "GL_DOUBLE_MAT4x2";
    case GL_DOUBLE_MAT4x3 : return "GL_DOUBLE_MAT4x3";
    case GL_SAMPLER_1D : return "GL_SAMPLER_1D";
    case GL_SAMPLER_2D : return "GL_SAMPLER_2D";
    case GL_SAMPLER_3D : return "GL_SAMPLER_3D";
    case GL_SAMPLER_CUBE : return "GL_SAMPLER_CUBE";
    case GL_SAMPLER_1D_SHADOW : return "GL_SAMPLER_1D_SHADOW";
    case GL_SAMPLER_2D_SHADOW : return "GL_SAMPLER_2D_SHADOW";
    case GL_SAMPLER_1D_ARRAY : return "GL_SAMPLER_1D_ARRAY";
    case GL_SAMPLER_2D_ARRAY : return "GL_SAMPLER_2D_ARRAY";
    case GL_SAMPLER_1D_ARRAY_SHADOW : return "GL_SAMPLER_1D_ARRAY_SHADOW";
    case GL_SAMPLER_2D_ARRAY_SHADOW : return "GL_SAMPLER_2D_ARRAY_SHADOW";
    case GL_SAMPLER_2D_MULTISAMPLE : return "GL_SAMPLER_2D_MULTISAMPLE";
    case GL_SAMPLER_2D_MULTISAMPLE_ARRAY : return "GL_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_SAMPLER_CUBE_SHADOW : return "GL_SAMPLER_CUBE_SHADOW";
    case GL_SAMPLER_BUFFER : return "GL_SAMPLER_BUFFER";
    case GL_SAMPLER_2D_RECT : return "GL_SAMPLER_2D_RECT";
    case GL_SAMPLER_2D_RECT_SHADOW : return "GL_SAMPLER_2D_RECT_SHADOW";
    case GL_INT_SAMPLER_1D : return "GL_INT_SAMPLER_1D";
    case GL_INT_SAMPLER_2D : return "GL_INT_SAMPLER_2D";
    case GL_INT_SAMPLER_3D : return "GL_INT_SAMPLER_3D";
    case GL_INT_SAMPLER_CUBE : return "GL_INT_SAMPLER_CUBE";
    case GL_INT_SAMPLER_1D_ARRAY : return "GL_INT_SAMPLER_1D_ARRAY";
    case GL_INT_SAMPLER_2D_ARRAY : return "GL_INT_SAMPLER_2D_ARRAY";
    case GL_INT_SAMPLER_2D_MULTISAMPLE : return "GL_INT_SAMPLER_2D_MULTISAMPLE";
    case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY : return "GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_INT_SAMPLER_BUFFER : return "GL_INT_SAMPLER_BUFFER";
    case GL_INT_SAMPLER_2D_RECT : return "GL_INT_SAMPLER_2D_RECT";
    case GL_UNSIGNED_INT_SAMPLER_1D : return "GL_UNSIGNED_INT_SAMPLER_1D";
    case GL_UNSIGNED_INT_SAMPLER_2D : return "GL_UNSIGNED_INT_SAMPLER_2D";
    case GL_UNSIGNED_INT_SAMPLER_3D : return "GL_UNSIGNED_INT_SAMPLER_3D";
    case GL_UNSIGNED_INT_SAMPLER_CUBE : return "GL_UNSIGNED_INT_SAMPLER_CUBE";
    case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY : return "GL_UNSIGNED_INT_SAMPLER_1D_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY : return "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE : return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE";
    case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY : return "GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY";
    case GL_UNSIGNED_INT_SAMPLER_BUFFER : return "GL_UNSIGNED_INT_SAMPLER_BUFFER";
    case GL_UNSIGNED_INT_SAMPLER_2D_RECT : return "GL_UNSIGNED_INT_SAMPLER_2D_RECT";
    case GL_IMAGE_1D : return "GL_IMAGE_1D";
    case GL_IMAGE_2D : return "GL_IMAGE_2D";
    case GL_IMAGE_3D : return "GL_IMAGE_3D";
    case GL_IMAGE_2D_RECT : return "GL_IMAGE_2D_RECT";
    case GL_IMAGE_CUBE : return "GL_IMAGE_CUBE";
    case GL_IMAGE_BUFFER : return "GL_IMAGE_BUFFER";
    case GL_IMAGE_1D_ARRAY : return "GL_IMAGE_1D_ARRAY";
    case GL_IMAGE_2D_ARRAY : return "GL_IMAGE_2D_ARRAY";
    case GL_IMAGE_2D_MULTISAMPLE : return "GL_IMAGE_2D_MULTISAMPLE";
    case GL_IMAGE_2D_MULTISAMPLE_ARRAY : return "GL_IMAGE_2D_MULTISAMPLE_ARRAY";
    case GL_INT_IMAGE_1D : return "GL_INT_IMAGE_1D";
    case GL_INT_IMAGE_2D : return "GL_INT_IMAGE_2D";
    case GL_INT_IMAGE_3D : return "GL_INT_IMAGE_3D";
    case GL_INT_IMAGE_2D_RECT : return "GL_INT_IMAGE_2D_RECT";
    case GL_INT_IMAGE_CUBE : return "GL_INT_IMAGE_CUBE";
    case GL_INT_IMAGE_BUFFER : return "GL_INT_IMAGE_BUFFER";
    case GL_INT_IMAGE_1D_ARRAY : return "GL_INT_IMAGE_1D_ARRAY";
    case GL_INT_IMAGE_2D_ARRAY : return "GL_INT_IMAGE_2D_ARRAY";
    case GL_INT_IMAGE_2D_MULTISAMPLE : return "GL_INT_IMAGE_2D_MULTISAMPLE";
    case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY : return "GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
    case GL_UNSIGNED_INT_IMAGE_1D : return "GL_UNSIGNED_INT_IMAGE_1D";
    case GL_UNSIGNED_INT_IMAGE_2D : return "GL_UNSIGNED_INT_IMAGE_2D";
    case GL_UNSIGNED_INT_IMAGE_3D : return "GL_UNSIGNED_INT_IMAGE_3D";
    case GL_UNSIGNED_INT_IMAGE_2D_RECT : return "GL_UNSIGNED_INT_IMAGE_2D_RECT";
    case GL_UNSIGNED_INT_IMAGE_CUBE : return "GL_UNSIGNED_INT_IMAGE_CUBE";
    case GL_UNSIGNED_INT_IMAGE_BUFFER : return "GL_UNSIGNED_INT_IMAGE_BUFFER";
    case GL_UNSIGNED_INT_IMAGE_1D_ARRAY : return "GL_UNSIGNED_INT_IMAGE_1D_ARRAY";
    case GL_UNSIGNED_INT_IMAGE_2D_ARRAY : return "GL_UNSIGNED_INT_IMAGE_2D_ARRAY";
    case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE : return "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE";
    case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY : return "GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY";
    case GL_UNSIGNED_INT_ATOMIC_COUNTER : return "GL_UNSIGNED_INT_ATOMIC_COUNTER";
    default : return "Not implemented conversion string";
  }
  
  
}

void checkUniformCallValidity(GLuint shaderProgram, GLuint index, GLenum desiredType) {
  const GLsizei bufSize = 50;
  GLsizei length;
  GLint size;
  GLenum type;
  GLchar name[bufSize];
  
  glCall(glGetActiveUniform(shaderProgram, index, bufSize, &length, &size, &type, name));
  
  if (type != desiredType) {
    std::stringstream err;
    err << "[SHADER] Invalid type passed to uniform variable ";
    err << name;
    err << ", expected: ";
    err << getGlenumName(type);
    err << ", called with: ";
    err << getGlenumName(desiredType);
    #if SHADERS_PEDANTIC_CHECKING
    throw std::runtime_error(err.str());
    #else
    spdlog::error(err.str());
    #endif
    
  }
}

Shader::Shader(const char *vertex, const char *fragment) : ShaderLoader() {
  spdlog::info("[SHADER] Creating shader");
  this->id = counterID++;
  shaderProgram = loadShader(vertex, fragment);
  checkCompilation();
  setModelMatrix(glm::mat4(1.f));
  camera_ = nullptr;
}

Shader::~Shader() {
  deleteShader();
}

void Shader::use(const std::shared_ptr<Material> &mtl) {
  static_cast<void>(mtl);
  if (currentShaderID != this->id) {
    currentShaderID = this->id;
    glCall(glUseProgram(shaderProgram));
  }
}

void Shader::setModelMatrix(glm::mat4 ModelMatrix) {
  this->modelMatrix = ModelMatrix;
  this->normalMatrix = glm::transpose(glm::inverse(glm::mat3(this->modelMatrix)));
}

void Shader::addCamera(const std::shared_ptr<Camera> &camera) {
  if (camera_ == nullptr) {
    camera_ = camera;
    //camera_->addShader(shared_from_this());
  }
}

void Shader::addLight(const std::shared_ptr<Light> &light) {
//  light->addShader(this);
  //printf("Adding light to shader %d"), this->id);
  lights_.emplace_back(light);
}

void Shader::addLight(const std::vector<std::shared_ptr<Light>> &light) {
  for (const auto &l : light) {
//    l->addShader(this);
    //printf("Adding light to shader %d"), this->id);
    lights_.emplace_back(l);
  }
}

void Shader::notify(const CameraInfo &c) {
  this->camInfo = c;
}

//void Shader::notify(const LightInfo &l) {
//  //this->lightInfo = l;
//
//}

void Shader::checkCompilation() {
  spdlog::info("[SHADER] Checking shader linker");
  GLint status;
  glCall(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status));
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glCall(glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength));
    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glCall(glGetProgramInfoLog(shaderProgram, infoLogLength, nullptr, strInfoLog));
    spdlog::error("[SHADER] Linker failure: {}", strInfoLog);
    delete[] strInfoLog;
  }
}

template<typename Tdata>
void Shader::setData(const std::string &name, const Tdata &value, const int overrideType) const {
  throw std::invalid_argument("Set data method not implemented for this data type.");
}

template<>
void Shader::setData<bool>(const std::string &name, const bool &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_BOOL);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1i(location, (int) value));
}

template<>
void Shader::setData<int>(const std::string &name, const int &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_INT);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1i(location, value));
}

template<>
void Shader::setData<float>(const std::string &name, const float &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1f(location, value));
}

template<>
void Shader::setData<double>(const std::string &name, const double &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_DOUBLE);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1d(location, value));
}

template<>
void Shader::setData<glm::vec3>(const std::string &name, const glm::vec3 &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_VEC3);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform3f(location, value.r, value.g, value.b));
}

template<>
void Shader::setData<glm::vec4>(const std::string &name, const glm::vec4 &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_VEC4);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform4f(location, value.r, value.g, value.b, value.a));
}

template<>
void Shader::setData<glm::mat4x4>(const std::string &name, const glm::mat4x4 &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_MAT4);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

template<>
void Shader::setData<glm::mat3x3>(const std::string &name, const glm::mat3x3 &value, const int overrideType) const {
  GLint location = glGetUniformLocation(id, name.c_str());
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_MAT3);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

template<typename Tdata>
void Shader::setData(const int location, const Tdata &value, const int overrideType) const {
  throw std::invalid_argument("Set data method not implemented for this data type.");
}

Shader::Shader(int shaderProgramStub) {
  shaderProgram = shaderProgramStub;
  spdlog::error("[SHADER] Creating null shader");
}

template<>
void Shader::setData<bool>(const int location, const bool &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_BOOL);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1i(location, (int) value));
}

template<>
void Shader::setData<int>(const int location, const int &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_INT);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1i(location, value));
}

template<>
void Shader::setData<float>(const int location, const float &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1f(location, value));
}

template<>
void Shader::setData<double>(const int location, const double &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_DOUBLE);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform1d(location, value));
}

template<>
void Shader::setData<glm::vec3>(const int location, const glm::vec3 &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_VEC3);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform3f(location, value.r, value.g, value.b));
}

template<>
void Shader::setData<glm::vec4>(const int location, const glm::vec4 &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_VEC4);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniform4f(location, value.r, value.g, value.b, value.a));
}

template<>
void Shader::setData<glm::mat4x4>(const int location, const glm::mat4x4 &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_MAT4);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

template<>
void Shader::setData<glm::mat3x3>(const int location, const glm::mat3x3 &value, const int overrideType) const {
  glCheckLocation(location >= 0);
  #if SHADERS_CHECKING
  if (overrideType > 0)
    checkUniformCallValidity(shaderProgram, location, overrideType);
  else
    checkUniformCallValidity(shaderProgram, location, GL_FLOAT_MAT3);
  #else
  static_cast<void>(overrideType);
  #endif
  glCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::BasicUniformLocations::init(int shaderProgram) {
  glCall(ModelTransform = glGetUniformLocation(shaderProgram, "u_modelMat"));
  glCall(ViewTransform = glGetUniformLocation(shaderProgram, "u_viewMat"));
  glCall(ProjectionTransform = glGetUniformLocation(shaderProgram, "u_projMat"));
  glCall(CameraPosition = glGetUniformLocation(shaderProgram, "u_cameraPosition"));
  
  if (ModelTransform == -1) { spdlog::warn("[SHADER]  ModelTransform not found"); }
  if (ViewTransform == -1) { spdlog::warn("[SHADER]  ViewTransform not found"); }
  if (ProjectionTransform == -1) { spdlog::warn("[SHADER]  ProjectionTransform not found"); }
  if (CameraPosition == -1) { spdlog::warn("[SHADER]  CameraPosition not found"); }
}

void Shader::MaterialUniformLocations::init(int shaderProgram) {
  glCall(MaterialDiffuseColor = glGetUniformLocation(shaderProgram, "u_material.diffuseColor"));
  glCall(MaterialDiffuseTexture = glGetUniformLocation(shaderProgram, "u_material.diffuseTexture"));
  glCall(MaterialHasDiffuseTexture = glGetUniformLocation(shaderProgram, "u_material.hasDiffuseTexture"));
  glCall(MaterialSpecularColor = glGetUniformLocation(shaderProgram, "u_material.specularColor"));
  glCall(MaterialSpecularTexture = glGetUniformLocation(shaderProgram, "u_material.specularTexture"));
  glCall(MaterialHasSpecularTexture = glGetUniformLocation(shaderProgram, "u_material.hasSpecularTexture"));
  glCall(MaterialNormalTexture = glGetUniformLocation(shaderProgram, "u_material.normalTexture"));
  glCall(MaterialHasNormalTexture = glGetUniformLocation(shaderProgram, "u_material.hasNormalTexture"));
  glCall(MaterialRoughness = glGetUniformLocation(shaderProgram, "u_material.roughness"));
  glCall(MaterialRoughnessTexture = glGetUniformLocation(shaderProgram, "u_material.roughnessTexture"));
  glCall(MaterialHasRoughnessTexture = glGetUniformLocation(shaderProgram, "u_material.hasRoughnessTexture"));
  glCall(MaterialMetalness = glGetUniformLocation(shaderProgram, "u_material.metalness"));
  glCall(MaterialMetalnessTexture = glGetUniformLocation(shaderProgram, "u_material.metalnessTexture"));
  glCall(MaterialHasMetalnessTexture = glGetUniformLocation(shaderProgram, "u_material.hasMetalnessTexture"));
  glCall(MaterialShininess = glGetUniformLocation(shaderProgram, "u_material.shininess"));
  glCall(MaterialIor = glGetUniformLocation(shaderProgram, "u_material.ior"));
  
  if (MaterialDiffuseColor == -1) { spdlog::warn("[SHADER]  MaterialDiffuseColor not found"); }
  if (MaterialDiffuseTexture == -1) { spdlog::warn("[SHADER]  MaterialDiffuseTexture not found"); }
  if (MaterialHasDiffuseTexture == -1) { spdlog::warn("[SHADER]  MaterialHasDiffuseTexture not found"); }
  
  if (MaterialSpecularColor == -1) { spdlog::warn("[SHADER]  MaterialSpecularColor not found"); }
  if (MaterialSpecularTexture == -1) { spdlog::warn("[SHADER]  MaterialSpecularTexture not found"); }
  if (MaterialHasSpecularTexture == -1) { spdlog::warn("[SHADER]  MaterialHasSpecularTexture not found"); }
  
  if (MaterialNormalTexture == -1) { spdlog::warn("[SHADER]  MaterialNormalTexture not found"); }
  if (MaterialHasNormalTexture == -1) { spdlog::warn("[SHADER]  MaterialHasNormalTexture not found"); }
  
  if (MaterialRoughness == -1) { spdlog::warn("[SHADER]  MaterialRoughness not found"); }
  if (MaterialRoughnessTexture == -1) { spdlog::warn("[SHADER]  MaterialRoughnessTexture not found"); }
  if (MaterialHasRoughnessTexture == -1) { spdlog::warn("[SHADER]  MaterialHasRoughnessTexture not found"); }
  
  if (MaterialMetalness == -1) { spdlog::warn("[SHADER]  MaterialMetalness not found"); }
  if (MaterialMetalnessTexture == -1) { spdlog::warn("[SHADER]  MaterialMetalnessTexture not found"); }
  if (MaterialHasMetalnessTexture == -1) { spdlog::warn("[SHADER]  MaterialHasMetalnessTexture not found"); }
  
  if (MaterialShininess == -1) { spdlog::warn("[SHADER]  MaterialShininess not found"); }
  if (MaterialIor == -1) { spdlog::warn("[SHADER]  MaterialIor not found"); }
}

void Shader::EnvironmentUniformLocations::init(int shaderProgram) {
  glCall(environmentMap = glGetUniformLocation(shaderProgram, "u_environment.environmentMap"));
  glCall(GGXIntegrMap = glGetUniformLocation(shaderProgram, "u_environment.GGXIntegrMap"));
  glCall(irradianceMap = glGetUniformLocation(shaderProgram, "u_environment.irradianceMap"));
  glCall(maxMip = glGetUniformLocation(shaderProgram, "u_environment.maxMip"));
  
  if (environmentMap == -1) { spdlog::warn("[SHADER] environmentMap not found"); }
  if (GGXIntegrMap == -1) { spdlog::warn("[SHADER] GGXIntegrMap not found"); }
  if (irradianceMap == -1) { spdlog::warn("[SHADER] irradianceMap not found"); }
  if (maxMip == -1) { spdlog::warn("[SHADER] maxMip not found"); }
}

void Shader::LightUniformLocations::init(int shaderProgram) {
  glCall(LightCount = glGetUniformLocation(shaderProgram, "u_lightCount"));
  if (LightCount == -1) { spdlog::warn("[SHADER] LightCount not found"); }
}

void Shader::LightUniformLocations::addLight(const std::shared_ptr<Light> &light, int shaderProgramId) {
  int idx = lightInfo.size();
  std::string number = std::to_string(idx);
  
  ShaderLightInfo phongLightInfo;
  glCall(phongLightInfo.position = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].position").c_str()));
  glCall(phongLightInfo.ambient = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].ambient").c_str()));
  glCall(phongLightInfo.diffuse = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].diffuse").c_str()));
  glCall(phongLightInfo.specular = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].specular").c_str()));
  glCall(phongLightInfo.constant = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].constant").c_str()));
  glCall(phongLightInfo.linear = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].linear").c_str()));
  glCall(phongLightInfo.quadratic = glGetUniformLocation(shaderProgramId, ("u_light[" + number + "].quadratic").c_str()));
  
  if (phongLightInfo.position == -1) { spdlog::warn("[SHADER]  lightInfo.position at index {} not found", idx); }
  if (phongLightInfo.ambient == -1) { spdlog::warn("[SHADER]  lightInfo.ambient at index {} not found", idx); }
  if (phongLightInfo.diffuse == -1) { spdlog::warn("[SHADER]  lightInfo.diffuse at index {} not found", idx); }
  if (phongLightInfo.specular == -1) { spdlog::warn("[SHADER]  lightInfo.specular at index {} not found", idx); }
  if (phongLightInfo.constant == -1) { spdlog::warn("[SHADER]  lightInfo.constant at index {} not found", idx); }
  if (phongLightInfo.linear == -1) { spdlog::warn("[SHADER]  lightInfo.linear at index {} not found", idx); }
  if (phongLightInfo.quadratic == -1) { spdlog::warn("[SHADER]  lightInfo.quadratic at index {} not found", idx); }
  
  lightInfo.emplace_back(phongLightInfo);
}

void Shader::LightUniformLocations::addLight(const std::vector<std::shared_ptr<Light>> &lights, int shaderProgramId) {
  for (const auto &light : lights) {
    addLight(light, shaderProgramId);
  }
}
