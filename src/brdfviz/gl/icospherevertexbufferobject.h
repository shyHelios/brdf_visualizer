/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef ICOSPHEREVERTEXBUFFEROBJECT_H
#define ICOSPHEREVERTEXBUFFEROBJECT_H

#include "vertexbufferobject.h"

class IcosphereVertexBufferObject : public VertexBufferObject {
public:
  IcosphereVertexBufferObject(int subdivisions, float radius = 1);

private:
  float radius;
  
  Vertex getHalfVertex(const Vertex &v1, const Vertex &v2);
};


#endif //ICOSPHEREVERTEXBUFFEROBJECT_H
