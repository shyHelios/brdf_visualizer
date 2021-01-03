/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef LINEVERTEXBUFFEROBJECT_H
#define LINEVERTEXBUFFEROBJECT_H

#include "vertexbufferobject.h"

class LineVertexBufferObject : public VertexBufferObject {
public:
  static LineVertexBufferObject *gizmo;
  
  static void setupStaticObjects();
  
  LineVertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, int lineWidth = 1);
  
  virtual void draw() override;
  
  virtual void drawLines() override;

protected:
  int lineWidth;
};


#endif //LINEVERTEXBUFFEROBJECT_H
