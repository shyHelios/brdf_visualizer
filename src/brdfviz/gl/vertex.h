/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef VERTEX_H
#define VERTEX_H

struct Vertex {
  Vertex() :
      Position{glm::vec3(0, 0, 0)},
      Normal{glm::vec3(0, 0, 0)},
      Texture{glm::vec2(0, 0)},
      Tangent{glm::vec3(0, 0, 0)} {
  }
  
  Vertex(
      float posx,
      float posy,
      float posz,
      float normx = 0.f,
      float normy = 0.f,
      float normz = 0.f,
      float texx = 0.f,
      float texy = 0.f,
      float tanx = 0.f,
      float tany = 0.f,
      float tanz = 0.f) :
      Position{glm::vec3(posx, posy, posz)},
      Normal{glm::vec3(normx, normy, normz)},
      Texture{glm::vec2(texx, texy)},
      Tangent{glm::vec3(tanx, tany, tanz)} {
  }
  
  Vertex(const glm::vec3 &position,
         const glm::vec3 &normal = glm::vec3(0.f, 0.f, 0.f),
         const glm::vec2 &texture = glm::vec2(0.f, 0.f),
         const glm::vec3 &tangent = glm::vec3(0.f, 0.f, 0.f)) :
      Position(position),
      Normal(normal),
      Texture(texture),
      Tangent(tangent) {}
  
  Vertex(const Vertex &vertex) : Position(vertex.Position),
                                 Normal(vertex.Normal),
                                 Texture(vertex.Texture),
                                 Tangent(vertex.Tangent) {};
  
  Vertex operator=(const Vertex &vertex) {
    return Vertex(vertex);
  }
  
  void log() {
    spdlog::info("[VERTEX] pos [{},{},{}]", Position[0], Position[1], Position[2]);
  }
  
  const glm::vec3 Position;
  const glm::vec3 Normal;
  const glm::vec2 Texture;
  const glm::vec3 Tangent;
};


#endif //VERTEX_H
