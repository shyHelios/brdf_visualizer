/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef RENDERER_H
#define RENDERER_H


class Renderer {
public:
  virtual void render(const bool geometry = false) = 0;

//  glm::vec4 clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
  glm::vec4 clearColor = glm::vec4(1.0f);
};


#endif //RENDERER_H
