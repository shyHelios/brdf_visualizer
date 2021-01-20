/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef LIGHT_H
#define LIGHT_H


#include "transformation.h"
#include "lightinfo.h"
//#include "object.h"
#include <vector>


#define LIGHT_INTENSITY_0  1.f, 0.7f, 1.8f
#define LIGHT_INTENSITY_1  1.f, 0.35f, 0.44f
#define LIGHT_INTENSITY_2  1.f, 0.22f, 0.20f
#define LIGHT_INTENSITY_3  1.f, 0.14f, 0.07f
#define LIGHT_INTENSITY_4  1.f, 0.09f, 0.032f
#define LIGHT_INTENSITY_5  1.f, 0.07f, 0.017f
#define LIGHT_INTENSITY_6  1.f, 0.045f, 0.0075f
#define LIGHT_INTENSITY_7  1.f, 0.027f, 0.0028f
#define LIGHT_INTENSITY_8  1.f, 0.022f, 0.0019f
#define LIGHT_INTENSITY_9  1.f, 0.014f, 0.0007f
#define LIGHT_INTENSITY_10  1.f, 0.007f, 0.0002f
#define LIGHT_INTENSITY_11  1.f, 0.0014f, 0.000007f


class Shader;

class Light {
public:
  Light(const std::shared_ptr<Transformation> &transformation, glm::vec4 diffuse = glm::vec4(.8f), glm::vec4 ambient = glm::vec4(0.1f),
        glm::vec4 specular = glm::vec4(.5f), float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
  
  Light(const std::shared_ptr<Transformation> &transformation, glm::vec4 diffuse = glm::vec4(.8f), float ambient = .1f, float specular = .5f,
        float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);


//  void addShader(Shader *shader);
  
  void update();

//  void notifyShaders();
  
  LightInfo getLigthInfo();

private:
  LightInfo lightInfo;
  bool changed;
  std::shared_ptr<Transformation> transformation;
//  std::vector<Shader *> shaders;
//  Shader *cdLights;
};


#endif //LIGHT_H
