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
//  static std::shared_ptr<VertexBufferObject> cube;
//  static std::shared_ptr<VertexBufferObject> plane;
//  static std::shared_ptr<VertexBufferObject> disk;
//  static std::shared_ptr<VertexBufferObject> icosahedron;
//  static std::shared_ptr<VertexBufferObject> halficosahedron;

//  static void setupStaticObjects();

//  static void deleteStaticObjects();

//  VertexBufferObject(const std::vector<float> &points);
  
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
