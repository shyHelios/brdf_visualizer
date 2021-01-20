/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "light.h"
#include <pch.h>
#include "shader.h"

Light::Light(const std::shared_ptr<Transformation> &transformation, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular, float constant,
             float linear, float quadratic) {
  this->transformation = transformation;
  
  this->lightInfo.position = transformation->getPosition();
  this->lightInfo.diffuse = diffuse;
  this->lightInfo.ambient = ambient;
  this->lightInfo.specular = specular;
  
  this->lightInfo.constant = constant;
  this->lightInfo.linear = linear;
  this->lightInfo.quadratic = quadratic;
  
  changed = false;
}

Light::Light(const std::shared_ptr<Transformation> &transformation, glm::vec4 diffuse, float ambient, float specular, float constant,
             float linear, float quadratic) {
  this->transformation = transformation;
  
  this->lightInfo.position = transformation->getPosition();
  this->lightInfo.diffuse = diffuse;
  this->lightInfo.ambient = diffuse * ambient;
  this->lightInfo.specular = diffuse * specular;
  
  this->lightInfo.constant = constant;
  this->lightInfo.linear = linear;
  this->lightInfo.quadratic = quadratic;
  
  changed = false;
}
//
//void Light::addShader(Shader *shader) {
//  this->shaders.emplace_back(shader);
//}

void Light::update() {
  
  /*float radius = 20.f;
  float lightX = lightInfo.position.x +sin(glfwGetTime()) * radius;
  float lightY = lightInfo.position.y +cos(glfwGetTime()) * radius;
  float lightZ = lightInfo.position.z +cos(glfwGetTime()) * sin(glfwGetTime()) * radius;
  lightInfo.position.x = lightX;
  lightInfo.position.x = lightY;
  lightInfo.position.x = lightZ*/
  ;
  
  /*LightInfo lightInfo = this->lightInfo;
  lightInfo.position.x = 0;
  lightInfo.position.y = 10;
  lightInfo.position.z = 0;*/
  
  /*lightInfo.position.x = lightX;
  lightInfo.position.y = lightY;
  lightInfo.position.z = lightZ;*/
  
  /*if (lightInfo.position != this->lightInfo.position) {
      this->lightInfo = lightInfo;
      notifyShaders();
  }*/
}
//
//void Light::notifyShaders() {
//  for (Shader *s : shaders) {
//    s->notify(this->lightInfo);
//  }
//}

LightInfo Light::getLigthInfo() {
  return this->lightInfo;
}
