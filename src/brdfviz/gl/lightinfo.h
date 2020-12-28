/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef LIGHTINFO_H
#define LIGHTINFO_H


struct LightInfo {
  glm::vec3 position;
  glm::vec4 diffuse = glm::vec4(.8f);
  glm::vec4 ambient = glm::vec4(.1f);
  glm::vec4 specular = glm::vec4(.5f);
  float constant = 1.f;
  float linear = .09f;
  float quadratic = .032f;
};


#endif //LIGHTINFO_H
