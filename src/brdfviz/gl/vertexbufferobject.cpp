/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "vertexbufferobject.h"
#include "vbos/cube.h"

VertexBufferObject *VertexBufferObject::cube = nullptr;


void VertexBufferObject::setupStaticObjects() {
  VertexBufferObject::cube = new VertexBufferObject(
      std::vector<Vertex>(std::begin(vbo::cube::vertices), std::end(vbo::cube::vertices)),
      std::vector<unsigned int>(std::begin(vbo::cube::indices), std::end(vbo::cube::indices)));
  
}

VertexBufferObject::VertexBufferObject(const std::vector<GLfloat> &points) {
  spdlog::info("[VBO] Creating model object");
  this->points = points;
  
  glCall(glGenVertexArrays(1, &VAO));
  glCall(glGenBuffers(1, &VBO));
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glCall(glBindVertexArray(VAO));
  
  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(points.data()) * points.size(), points.data(), GL_STATIC_DRAW));
  
  // position attribute
  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(points.data()), (void *) 0));
  glCall(glEnableVertexAttribArray(0));
  // normal attribute
  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(points.data()), (void *) (3 * sizeof(points.data()))));
  glCall(glEnableVertexAttribArray(1));
}

VertexBufferObject::VertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) : indices(indices), vertices(vertices) {
//  this->indices = indices;
//  this->vertices = vertices;
  glCall(glGenVertexArrays(1, &VAO));
  glCall(glGenBuffers(1, &VBO));
  glCall(glGenBuffers(1, &IBO));
  
  glCall(glBindVertexArray(VAO));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  
  glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
  
  glCall(glEnableVertexAttribArray(0));
  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (0)));
  glCall(glEnableVertexAttribArray(1));
  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (3 * sizeof(GLfloat))));
  glCall(glEnableVertexAttribArray(2));
  glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (6 * sizeof(GLfloat))));
  glCall(glEnableVertexAttribArray(3));
  glCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (8 * sizeof(GLfloat))));
  
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));
  
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  glCall(glBindVertexArray(VAO));
}

VertexBufferObject::~VertexBufferObject() {
  printf("VertexBufferObject destructor called\n");
}

void VertexBufferObject::draw() {
  glCall(glBindVertexArray(VAO)); //Draw VAO
  //glDrawArrays(GL_TRIANGLES, 0, points.size() / 6); //mode, first, count
  glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL));
}