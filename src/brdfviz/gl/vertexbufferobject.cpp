/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include "vertexbufferobject.h"
#include "vbos/cube.h"
#include "vbos/plane.h"
#include "vbos/icosahedron.h"
#include "vbos/halficosahedron.h"

VertexBufferObject *VertexBufferObject::cube = nullptr;
VertexBufferObject *VertexBufferObject::plane = nullptr;
VertexBufferObject *VertexBufferObject::disk = nullptr;
VertexBufferObject *VertexBufferObject::icosahedron = nullptr;
VertexBufferObject *VertexBufferObject::halficosahedron = nullptr;


void VertexBufferObject::setupStaticObjects() {
  VertexBufferObject::cube = new VertexBufferObject(
      std::vector<Vertex>(std::begin(vbo::cube::vertices), std::end(vbo::cube::vertices)),
      std::vector<unsigned int>(std::begin(vbo::cube::indices), std::end(vbo::cube::indices)));
  
  VertexBufferObject::plane = new VertexBufferObject(
      std::vector<Vertex>(std::begin(vbo::plane::vertices), std::end(vbo::plane::vertices)),
      std::vector<unsigned int>(std::begin(vbo::plane::indices), std::end(vbo::plane::indices)));
  
  VertexBufferObject::icosahedron = new VertexBufferObject(
      std::vector<Vertex>(std::begin(vbo::icosahedron::vertices), std::end(vbo::icosahedron::vertices)),
      std::vector<unsigned int>(std::begin(vbo::icosahedron::indices), std::end(vbo::icosahedron::indices)));
  
  VertexBufferObject::halficosahedron = new VertexBufferObject(
      std::vector<Vertex>(std::begin(vbo::halficosahedron::vertices), std::end(vbo::halficosahedron::vertices)),
      std::vector<unsigned int>(std::begin(vbo::halficosahedron::indices), std::end(vbo::halficosahedron::indices)));
  
  //generate disk
  {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const unsigned int resolution = 20;
    double radiansAngle = 0;
    const double radiansFullAngle = 2. * M_PI;
    const double radius = 1.;
    const double radiansStep = radiansFullAngle / static_cast<double>(resolution);
    vertices.emplace_back(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // center
    for (int i = 0; i < resolution; i++) {
      vertices.emplace_back(glm::vec3(radius * std::cos(radiansAngle), radius * std::sin(radiansAngle), 0), glm::vec3(0, 1, 0));
//      vertices.emplace_back(glm::vec3(radius * std::cos(radiansAngle), 0, radius * std::sin(radiansAngle)), glm::vec3(std::cos(radiansAngle), 0, std::sin(radiansAngle)));
      radiansAngle += radiansStep;

//      int vidx1 = i;
//      int vidx2 = (i + 1) % resolution;
      indices.emplace_back(0);
      indices.emplace_back(((i + 1) % resolution) + 1);
      indices.emplace_back(i + 1);
    }
    
    VertexBufferObject::disk = new VertexBufferObject(vertices, indices);
  }
}

//VertexBufferObject::VertexBufferObject(const std::vector<GLfloat> &points) {
//  spdlog::info("[VBO] Creating model object");
//  this->points = points;
//
//  glCall(glGenVertexArrays(1, &VAO));
//  glCall(glGenBuffers(1, &VBO));
//  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//  glCall(glBindVertexArray(VAO));
//
//  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
//  glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(points.data()) * points.size(), points.data(), GL_STATIC_DRAW));
//
//  // position attribute
//  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(points.data()), (void *) 0));
//  glCall(glEnableVertexAttribArray(0));
//  // normal attribute
//  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(points.data()), (void *) (3 * sizeof(points.data()))));
//  glCall(glEnableVertexAttribArray(1));
//}

VertexBufferObject::VertexBufferObject(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) :
    indices(indices),
    vertices(vertices) {
  
  glCall(glGenVertexArrays(1, &VAO));
  glCall(glGenBuffers(1, &VBO));
  glCall(glGenBuffers(1, &IBO));
  
  glCall(glBindVertexArray(VAO));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  
  glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
  
  // Position
  glCall(glEnableVertexAttribArray(0));
  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (0)));
  //Normal
  glCall(glEnableVertexAttribArray(1));
  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (3 * sizeof(GLfloat))));
  //Texture
  glCall(glEnableVertexAttribArray(2));
  glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (6 * sizeof(GLfloat))));
  //Tangent
  glCall(glEnableVertexAttribArray(3));
  glCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (8 * sizeof(GLfloat))));
  
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));
  
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  glCall(glBindVertexArray(0));
}

VertexBufferObject::~VertexBufferObject() {
  printf("VertexBufferObject destructor called\n");
  glCall(glDeleteVertexArrays(1, &VAO));
  glCall(glDeleteBuffers(1, &VBO));
  glCall(glDeleteBuffers(1, &IBO));
}

void VertexBufferObject::draw() {
  glCall(glBindVertexArray(VAO));
//  glCall(glLineWidth(5));
//  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
  glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL));
//  glCall(glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL));
  glCall(glBindVertexArray(0));
  
}

void VertexBufferObject::drawLines() {
  glCall(glBindVertexArray(VAO));
  glCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
  glCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL));
//  glCall(glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL));
  glCall(glBindVertexArray(0));
}

void VertexBufferObject::recreate(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices) {
  if (VAO > 0) {
    glCall(glDeleteVertexArrays(1, &VAO));
    glCall(glDeleteBuffers(1, &VBO));
    glCall(glDeleteBuffers(1, &IBO));
  }
  
  glCall(glGenVertexArrays(1, &VAO));
  glCall(glGenBuffers(1, &VBO));
  glCall(glGenBuffers(1, &IBO));
  
  glCall(glBindVertexArray(VAO));
  glCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  
  glCall(glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW));
  
  // Position
  glCall(glEnableVertexAttribArray(0));
  glCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (0)));
  //Normal
  glCall(glEnableVertexAttribArray(1));
  glCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (3 * sizeof(GLfloat))));
  //Texture
  glCall(glEnableVertexAttribArray(2));
  glCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (6 * sizeof(GLfloat))));
  //Tangent
  glCall(glEnableVertexAttribArray(3));
  glCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) (8 * sizeof(GLfloat))));
  
  glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));
  glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW));
  
  glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  glCall(glBindVertexArray(0));
}
