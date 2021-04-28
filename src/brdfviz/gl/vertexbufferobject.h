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
  VertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  
  virtual ~VertexBufferObject();
  
  void recreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  
  virtual void draw();
  
  virtual void drawLines();

protected:
  std::vector<Vertex> vertices_;
  std::vector<unsigned int> indices_;
  unsigned int VBO = 0;
  unsigned int VAO = 0;
  unsigned int IBO = 0;
};


#endif //VERTEXBUFFEROBJECT_H
