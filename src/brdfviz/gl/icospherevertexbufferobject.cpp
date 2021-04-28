/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "icospherevertexbufferobject.h"
#include "vbos/icosahedron.h"
#include "vbos/halficosahedron.h"
#include "vbos/plane.h"

IcosphereVertexBufferObject::IcosphereVertexBufferObject(int subdivisions, float radius) :
    radius(radius),
//    VertexBufferObject(std::vector<Vertex>(std::begin(vbo::icosahedron::vertices), std::end(vbo::icosahedron::vertices)),
//                       std::vector<unsigned int>(std::begin(vbo::icosahedron::indices), std::end(vbo::icosahedron::indices)))
    VertexBufferObject(std::vector<Vertex>(std::begin(vbo::halficosahedron::vertices), std::end(vbo::halficosahedron::vertices)),
                       std::vector<unsigned int>(std::begin(vbo::halficosahedron::indices), std::end(vbo::halficosahedron::indices)))
//    VertexBufferObject(std::vector<Vertex>(std::begin(vbo::plane::vertices), std::end(vbo::plane::vertices)),
//                       std::vector<unsigned int>(std::begin(vbo::plane::indices), std::end(vbo::plane::indices)))
{
  std::vector<Vertex> tmpVertices;
  std::vector<unsigned int> tmpIndices;
  unsigned int index = 0;
  
  for (int i = 0; i < subdivisions; i++) {
    tmpVertices = vertices_;
    tmpIndices = indices_;
    
    vertices_.clear();
    indices_.clear();
    
    index = 0;
    
    for (int j = 0; j < tmpIndices.size(); j += 3) {
      // get 3 vertices of a triangle
      const Vertex v1 = tmpVertices[tmpIndices[j]];
      const Vertex v2 = tmpVertices[tmpIndices[j + 1]];
      const Vertex v3 = tmpVertices[tmpIndices[j + 2]];
      
      const Vertex v12 = getHalfVertex(v1, v2);
      const Vertex v23 = getHalfVertex(v2, v3);
      const Vertex v31 = getHalfVertex(v3, v1);
      
      vertices_.emplace_back(v1);
      vertices_.emplace_back(v12);
      vertices_.emplace_back(v2);
      vertices_.emplace_back(v23);
      vertices_.emplace_back(v3);
      vertices_.emplace_back(v31);
      
      indices_.emplace_back(index + 0);
      indices_.emplace_back(index + 1);
      indices_.emplace_back(index + 5);
      
      indices_.emplace_back(index + 1);
      indices_.emplace_back(index + 2);
      indices_.emplace_back(index + 3);
      
      indices_.emplace_back(index + 5);
      indices_.emplace_back(index + 1);
      indices_.emplace_back(index + 3);
      
      indices_.emplace_back(index + 5);
      indices_.emplace_back(index + 3);
      indices_.emplace_back(index + 4);
      
      index += 6;
    }
  }
  
  recreate(vertices_, indices_);
}

Vertex IcosphereVertexBufferObject::getHalfVertex(const Vertex &v1, const Vertex &v2) {
  glm::vec3 halfVecPos = (v1.Position + v2.Position) / 2.f;
  halfVecPos = glm::normalize(halfVecPos) * radius;
  
  glm::vec3 halfVecNormal = glm::normalize(v1.Normal + v2.Normal);
  glm::vec3 halfVecTangent = (v1.Tangent + v2.Tangent) / 2.f;
  glm::vec2 halfVecTexcoord = (v1.Texture + v2.Texture) / 2.f;
  
  return Vertex(halfVecPos, halfVecNormal, halfVecTexcoord, halfVecTangent);
}
