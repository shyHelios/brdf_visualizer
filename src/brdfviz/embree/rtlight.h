/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RTLIGHT_H
#define RTLIGHT_H


class RTLight {
public:
  RTLight(const glm::vec3 &position, const glm::vec3 &color = glm::vec3(1, 1, 1));
  
  const glm::vec3 &getPosition() const;
  
  const glm::vec3 &getColor() const;

private:
  glm::vec3 position_;
  glm::vec3 color_;
};


#endif //RTLIGHT_H
