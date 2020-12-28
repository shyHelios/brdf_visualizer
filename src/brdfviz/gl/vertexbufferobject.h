/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef VERTEXBUFFEROBJECT_H
#define VERTEXBUFFEROBJECT_H

#include <vector>
#include "vertex.h"

class VertexBufferObject {
public:
  static VertexBufferObject *cube;
  
  static void setupStaticObjects();
  
  VertexBufferObject(const std::vector<float> &points);
  
  VertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  
  ~VertexBufferObject();
  
  void draw();

private:
  std::vector<float> points;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VBO;
  unsigned int VAO;
  unsigned int IBO;
};


#endif //VERTEXBUFFEROBJECT_H
