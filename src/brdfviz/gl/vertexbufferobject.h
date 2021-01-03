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
  static VertexBufferObject *plane;
  static VertexBufferObject *disk;
  static VertexBufferObject *icosahedron;
  static VertexBufferObject *halficosahedron;
  
  static void setupStaticObjects();

//  VertexBufferObject(const std::vector<float> &points);
  
  VertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  
  ~VertexBufferObject();
  
  void recreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices);
  
  virtual void draw();
  
  virtual void drawLines();

protected:
//  std::vector<float> points;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  unsigned int VBO = 0;
  unsigned int VAO = 0;
  unsigned int IBO = 0;
};


#endif //VERTEXBUFFEROBJECT_H
